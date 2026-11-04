/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "charger-cool-down.h"

#include <aidl/vendor/oplus/hardware/charger/ICharger.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/strings.h>
#include <android/binder_manager.h>

#include <dirent.h>

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>

using aidl::vendor::oplus::hardware::charger::ICharger;
using android::base::ParseInt;
using android::base::ReadFileToString;
using android::base::Split;
using android::base::StartsWith;
using android::base::Trim;

namespace oplus::power {

namespace {

constexpr char kConfigPath[] = "/vendor/etc/charger_cool_down.conf";
constexpr char kThermalClassPath[] = "/sys/class/thermal";
// Stock passes the foreground package, which the kernel only tracks.
constexpr char kPackageName[] = "android";
constexpr auto kPollInterval = std::chrono::seconds(3);

struct Config {
    std::vector<std::string> sensors;
    // Shell temperature in millidegrees Celsius at which each level starts.
    std::vector<int> thresholds;
    // Shell temperature in millidegrees Celsius below which each level ends.
    std::vector<int> thresholdsClr;
    std::vector<int> levels;
};

std::optional<Config> readConfig() {
    std::string content;
    if (!ReadFileToString(kConfigPath, &content)) {
        return std::nullopt;
    }

    Config config;
    for (const auto& line : Split(content, "\n")) {
        std::vector<std::string> values = Split(Trim(line), " ");
        const std::string key = values[0];
        values.erase(values.begin());

        if (key.empty()) {
            continue;
        } else if (key == "sensors") {
            config.sensors = values;
            continue;
        }

        std::vector<int>* target = nullptr;
        if (key == "thresholds") {
            target = &config.thresholds;
        } else if (key == "thresholds_clr") {
            target = &config.thresholdsClr;
        } else if (key == "levels") {
            target = &config.levels;
        } else {
            LOG(WARNING) << "Ignoring unknown key " << key << " in " << kConfigPath;
            continue;
        }

        for (const auto& value : values) {
            int parsed;
            if (!ParseInt(value, &parsed)) {
                LOG(ERROR) << "Invalid value " << value << " for " << key << " in " << kConfigPath;
                return std::nullopt;
            }
            target->push_back(parsed);
        }
    }

    if (config.levels.empty()) {
        return std::nullopt;
    }

    if (config.sensors.empty() || config.thresholds.size() != config.levels.size() ||
        config.thresholdsClr.size() != config.levels.size()) {
        LOG(ERROR) << "Invalid " << kConfigPath;
        return std::nullopt;
    }

    return config;
}

class ChargerCoolDown {
  public:
    static ChargerCoolDown* getInstance() {
        static ChargerCoolDown* instance = []() -> ChargerCoolDown* {
            auto config = readConfig();
            return config ? new ChargerCoolDown(std::move(*config)) : nullptr;
        }();
        return instance;
    }

    void setActive(bool active) {
        {
            std::lock_guard lock(mutex_);
            active_ = active;
        }
        cv_.notify_one();
    }

  private:
    explicit ChargerCoolDown(Config config) : config_(std::move(config)) {
        LOG(INFO) << "Loaded " << config_.levels.size() << " charger cool down levels";
        std::thread(&ChargerCoolDown::run, this).detach();
    }

    void run() {
        std::unique_lock lock(mutex_);
        while (true) {
            if (!active_) {
                lock.unlock();
                index_ = -1;
                applyLevel(0);
                lock.lock();
                cv_.wait(lock, [this] { return active_; });
                continue;
            }

            lock.unlock();
            update();
            lock.lock();
            cv_.wait_for(lock, kPollInterval, [this] { return !active_; });
        }
    }

    void update() {
        auto charger = getCharger();
        if (!charger) {
            return;
        }

        int online = 0;
        if (!charger->getPsyUsbOnline(&online).isOk()) {
            charger_.reset();
            return;
        }

        if (online <= 0) {
            index_ = -1;
        } else if (auto temp = readTemperature()) {
            const int count = config_.levels.size();
            while (index_ + 1 < count && *temp >= config_.thresholds[index_ + 1]) {
                index_++;
            }
            while (index_ >= 0 && *temp < config_.thresholdsClr[index_]) {
                index_--;
            }
        }

        applyLevel(index_ >= 0 ? config_.levels[index_] : 0);
    }

    void applyLevel(int level) {
        if (level == appliedLevel_) {
            return;
        }

        auto charger = getCharger();
        if (!charger) {
            return;
        }

        int ret = 0;
        if (!charger->setSmartCoolDown(level, level, kPackageName, &ret).isOk()) {
            LOG(ERROR) << "Failed to set charger cool down level " << level;
            charger_.reset();
            return;
        }

        LOG(INFO) << "Charger cool down level " << level << " (ret " << ret << ")";
        appliedLevel_ = level;
    }

    std::shared_ptr<ICharger> getCharger() {
        if (!charger_) {
            const std::string instance = std::string() + ICharger::descriptor + "/default";
            charger_ = ICharger::fromBinder(
                    ndk::SpAIBinder(AServiceManager_checkService(instance.c_str())));
        }
        return charger_;
    }

    std::optional<int> readTemperature() {
        if (sensorPaths_.empty()) {
            findSensorPaths();
        }

        std::optional<int> maxTemp;
        for (const auto& path : sensorPaths_) {
            std::string content;
            int temp;
            if (ReadFileToString(path, &content) && ParseInt(Trim(content), &temp)) {
                maxTemp = std::max(maxTemp.value_or(temp), temp);
            }
        }

        if (!maxTemp) {
            sensorPaths_.clear();
        }

        return maxTemp;
    }

    void findSensorPaths() {
        std::unique_ptr<DIR, decltype(&closedir)> dir(opendir(kThermalClassPath), closedir);
        if (!dir) {
            PLOG(ERROR) << "Failed to open " << kThermalClassPath;
            return;
        }

        while (dirent* entry = readdir(dir.get())) {
            if (!StartsWith(entry->d_name, "thermal_zone")) {
                continue;
            }

            const std::string zone = std::string(kThermalClassPath) + "/" + entry->d_name;
            std::string type;
            if (ReadFileToString(zone + "/type", &type) &&
                std::ranges::find(config_.sensors, Trim(type)) != config_.sensors.end()) {
                sensorPaths_.push_back(zone + "/temp");
            }
        }
    }

    const Config config_;

    std::mutex mutex_;
    std::condition_variable cv_;
    bool active_ = false;

    std::shared_ptr<ICharger> charger_;
    std::vector<std::string> sensorPaths_;
    int index_ = -1;
    int appliedLevel_ = -1;
};

}  // namespace

void setChargerCoolDownActive(bool active) {
    if (auto instance = ChargerCoolDown::getInstance()) {
        instance->setActive(active);
    }
}

}  // namespace oplus::power
