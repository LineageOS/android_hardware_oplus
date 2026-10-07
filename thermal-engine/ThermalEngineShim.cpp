/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "libshims_thermal-engine.oplus"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <json/reader.h>
#include <json/value.h>
#include <openssl/aes.h>
#include <tinyxml2.h>

#include <dlfcn.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include <algorithm>
#include <climits>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

using android::base::Join;
using android::base::ParseInt;
using android::base::ReadFileToString;
using android::base::Split;
using android::base::StringAppendF;
using android::base::StringPrintf;
using android::base::Trim;
using android::base::WriteStringToFd;

namespace {

constexpr char kThermalEngineConfig[] = "/vendor/etc/thermal-engine.conf";
constexpr char kThermalControlConfig[] =
        "/odm/etc/temperature_profile/sys_thermal_control_config.xml";
constexpr char kHoraeTargetConfig[] = "/odm/etc/horae/horae_target.conf";
constexpr char kProjectName[] = "/proc/oplusVersion/prjName";

constexpr uint8_t kHoraeKey[] = {0x00, 0x01, 0x0a, 0xae, 0x69, 0xca, 0x90, 0xb7,
                                 0x4e, 0x9f, 0xfa, 0x45, 0xe3, 0x16, 0xd7, 0x52};

// Size of the threshold array in thermal-engine-v2 monitor settings.
constexpr size_t kMaxThresholds = 12;
constexpr int kNoLimit = INT_MAX;
constexpr int kSamplingMs = 1000;

struct StatusModel {
    std::vector<int> up;
    std::vector<int> down;
    std::vector<int> status;
};

// thermal_status_model from /system_ext/etc/horae/horae.conf, horae_target.conf may override it.
const StatusModel kDefaultStatusModel = {
        .up = {25, 27, 29, 31, 33, 35, 37, 39, 40, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63},
        .down = {23, 25, 27, 29, 31, 33, 35, 37, 39, 40, 41,
                 43, 45, 47, 49, 51, 53, 55, 57, 59, 61},
        .status = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19},
};

struct Gear {
    int cpu;
    int gpu;
};

struct ControlConfig {
    std::map<int, std::vector<int>> cpuLevels;
    std::map<int, int> gpuLevels;
    std::map<int, Gear> gears;
    size_t clusters = 0;
};

struct Level {
    int temp;
    int clr;
    std::vector<int> limits;
};

std::optional<Json::Value> ReadHoraeConfig(const char* path) {
    std::string data;
    if (!ReadFileToString(path, &data) || data.empty() || data.size() % AES_BLOCK_SIZE) {
        LOG(ERROR) << "Failed to read " << path;
        return std::nullopt;
    }

    AES_KEY key;
    AES_set_decrypt_key(kHoraeKey, sizeof(kHoraeKey) * 8, &key);
    uint8_t iv[AES_BLOCK_SIZE] = {};
    std::string plain(data.size(), '\0');
    AES_cbc_encrypt(reinterpret_cast<const uint8_t*>(data.data()),
                    reinterpret_cast<uint8_t*>(plain.data()), data.size(), &key, iv, AES_DECRYPT);

    // Plaintext is <u32 LE length><JSON><junk>.
    uint32_t length;
    memcpy(&length, plain.data(), sizeof(length));
    if (length > plain.size() - sizeof(length)) {
        LOG(ERROR) << "Failed to decrypt " << path;
        return std::nullopt;
    }

    Json::Value root;
    std::string errors;
    std::unique_ptr<Json::CharReader> reader(Json::CharReaderBuilder().newCharReader());
    const char* begin = plain.data() + sizeof(length);
    if (!reader->parse(begin, begin + length, &root, &errors) || !root.isObject()) {
        LOG(ERROR) << "Failed to parse " << path << ": " << errors;
        return std::nullopt;
    }

    return root;
}

const Json::Value& SelectProject(const Json::Value& root) {
    std::string project;
    if (ReadFileToString(kProjectName, &project)) {
        project = Trim(project);
        if (root[project].isObject()) {
            return root[project];
        }
    }

    for (const auto& name : root.getMemberNames()) {
        if (root[name].isObject() && root[name].isMember("shell_temp_fitting_model")) {
            return root[name];
        }
    }

    return root;
}

std::vector<std::string> GetShellSensors(const Json::Value& project) {
    const auto& models = project["shell_temp_fitting_model"]["default"]["temp_model"];
    std::set<std::string> sensors;

    for (const auto& name : models.getMemberNames()) {
        // Negative index (board) doesn't feed a shell zone, <name>_N are extra hot spots of <name>.
        if (models[name]["index"].asInt() >= 0) {
            sensors.insert("shell_" + name.substr(0, name.find('_')));
        }
    }

    return {sensors.begin(), sensors.end()};
}

std::optional<StatusModel> ParseStatusModel(const Json::Value& value) {
    if (!value.isObject()) {
        return std::nullopt;
    }

    StatusModel model;
    for (const auto& e : value["temp_threshold"]) model.up.push_back(e.asInt());
    for (const auto& e : value["temp_threshold_down"]) model.down.push_back(e.asInt());
    for (const auto& e : value["thermal_status"]) model.status.push_back(e.asInt());

    if (model.status.empty() || model.up.size() != model.status.size() ||
        model.down.size() != model.status.size()) {
        LOG(ERROR) << "Invalid thermal_status_model";
        return std::nullopt;
    }

    return model;
}

std::optional<ControlConfig> ReadControlConfig(const char* path) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(path) != tinyxml2::XML_SUCCESS) {
        LOG(ERROR) << "Failed to parse " << path;
        return std::nullopt;
    }

    ControlConfig config;
    tinyxml2::XMLHandle root(doc.FirstChildElement("sys_thermal_control_list"));

    for (auto feature = root.FirstChildElement("feature").ToElement(); feature;
         feature = feature->NextSiblingElement("feature")) {
        for (auto c = feature->FirstChildElement("config"); c;
             c = c->NextSiblingElement("config")) {
            for (auto level = c->FirstChildElement("level"); level;
                 level = level->NextSiblingElement("level")) {
                int index = level->IntAttribute("index", -1);

                if (c->Attribute("name", "thermal_cpulevel")) {
                    std::vector<int> freqs;
                    for (const auto& freq : Split(level->Attribute("maxCpuFreq") ?: "", ",")) {
                        int value;
                        freqs.push_back(ParseInt(freq, &value) ? value : -1);
                    }
                    config.clusters = std::max(config.clusters, freqs.size());
                    config.cpuLevels[index] = std::move(freqs);
                } else if (c->Attribute("name", "thermal_gpulevel")) {
                    config.gpuLevels[index] = level->IntAttribute("maxGpuFreq", -1);
                }
            }
        }
    }

    auto policy = root.FirstChildElement("thermalPolicyConfigItem")
                          .FirstChildElement("globalPolicy")
                          .FirstChildElement("globalPolicy")
                          .ToElement();
    for (auto gear = policy ? policy->FirstChildElement("gear_config") : nullptr; gear;
         gear = gear->NextSiblingElement("gear_config")) {
        config.gears[gear->IntAttribute("tempGear", -1)] = {
                .cpu = gear->IntAttribute("cpu", -1),
                .gpu = gear->IntAttribute("gpu", -1),
        };
    }

    if (config.gears.empty()) {
        LOG(ERROR) << "No globalPolicy in " << path;
        return std::nullopt;
    }

    return config;
}

std::vector<Level> BuildLevels(const ControlConfig& control, const StatusModel& model) {
    std::vector<Level> levels;

    for (size_t i = 0; i < model.status.size(); i++) {
        if (model.up[i] < 35) {
            continue;
        }

        auto gear = control.gears.find(model.status[i]);
        if (gear == control.gears.end()) {
            continue;
        }

        std::vector<int> limits(control.clusters, kNoLimit);
        if (auto cpu = control.cpuLevels.find(gear->second.cpu); cpu != control.cpuLevels.end()) {
            for (size_t c = 0; c < cpu->second.size(); c++) {
                if (cpu->second[c] >= 0) limits[c] = cpu->second[c];
            }
        }
        auto gpu = control.gpuLevels.find(gear->second.gpu);
        limits.push_back(gpu != control.gpuLevels.end() && gpu->second >= 0 ? gpu->second
                                                                            : kNoLimit);

        bool redundant = levels.empty() ? std::all_of(limits.begin(), limits.end(),
                                                      [](int l) { return l == kNoLimit; })
                                        : levels.back().limits == limits;
        if (!redundant) {
            levels.push_back({model.up[i] * 1000, model.down[i] * 1000, std::move(limits)});
        }
    }

    return levels;
}

std::string GenerateConfig() {
    auto control = ReadControlConfig(kThermalControlConfig);
    auto horae = ReadHoraeConfig(kHoraeTargetConfig);
    if (!control || !horae) {
        return {};
    }

    const auto& project = SelectProject(*horae);
    auto sensors = GetShellSensors(project);
    if (sensors.empty()) {
        LOG(ERROR) << "No shell temp fitting models in " << kHoraeTargetConfig;
        return {};
    }

    auto model = ParseStatusModel(project["thermal_status_model"]);
    if (!model) model = ParseStatusModel((*horae)["thermal_status_model"]);
    if (!model) model = kDefaultStatusModel;

    auto levels = BuildLevels(*control, *model);
    if (levels.empty()) {
        LOG(ERROR) << "No thermal levels in " << kThermalControlConfig;
        return {};
    }

    std::vector<std::string> devices;
    for (size_t c = 0; c < control->clusters; c++) {
        devices.push_back(StringPrintf("cpu-cluster%zu", c));
    }
    devices.push_back("gpu");
    std::string actions = Join(devices, '+');

    std::string config;
    for (const auto& sensor : sensors) {
        // Levels past kMaxThresholds go to extra monitors, thermal-engine applies the lowest
        // requested limit per device.
        for (size_t first = 0; first < levels.size(); first += kMaxThresholds) {
            std::string thresholds = "thresholds";
            std::string thresholdsClr = "thresholds_clr";
            std::string levelActions = "actions";
            std::string actionInfo = "action_info";

            for (size_t i = first; i < std::min(levels.size(), first + kMaxThresholds); i++) {
                StringAppendF(&thresholds, " %d", levels[i].temp);
                StringAppendF(&thresholdsClr, " %d", levels[i].clr);
                levelActions += " " + actions;
                actionInfo += " " + Join(levels[i].limits, '+');
            }

            StringAppendF(&config, "[%s-%zu]\n", sensor.c_str(), first / kMaxThresholds);
            StringAppendF(&config, "algo_type monitor\nsensor %s\nsampling %d\n", sensor.c_str(),
                          kSamplingMs);
            config += thresholds + "\n" + thresholdsClr + "\n" + levelActions + "\n" + actionInfo +
                      "\n\n";
        }
    }

    LOG(INFO) << "Generated " << levels.size() << " levels for " << Join(sensors, ',');
    return config;
}

FILE* OpenGeneratedConfig(const char* mode) {
    std::string config = GenerateConfig();
    if (config.empty()) {
        return nullptr;
    }

    int fd = memfd_create("thermal-engine.conf", MFD_CLOEXEC);
    if (fd < 0 || !WriteStringToFd(config, fd) || lseek(fd, 0, SEEK_SET) < 0) {
        PLOG(ERROR) << "Failed to create in-memory config";
        return nullptr;
    }

    return fdopen(fd, mode);
}

}  // namespace

extern "C" FILE* fopen(const char* path, const char* mode) {
    static auto fopen_orig = reinterpret_cast<decltype(fopen)*>(dlsym(RTLD_NEXT, "fopen"));

    if (path && strcmp(path, kThermalEngineConfig) == 0) {
        if (FILE* fp = OpenGeneratedConfig(mode)) {
            return fp;
        }
    }

    return fopen_orig(path, mode);
}
