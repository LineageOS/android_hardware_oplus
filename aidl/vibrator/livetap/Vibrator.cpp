/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Vibrator.h"

#include <fcntl.h>
#include <log/log.h>
#include <stdio.h>
#include <unistd.h>

#include <cerrno>
#include <chrono>
#include <cmath>
#include <cstring>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#ifndef LIVETAP_DEFAULT_F0
#define LIVETAP_DEFAULT_F0 170
#endif

namespace aidl {
namespace android {
namespace hardware {
namespace vibrator {

namespace {

constexpr int32_t kMinLevel = 100;
constexpr int32_t kMaxLevel = 2400;
constexpr int32_t kDefaultOnLevel = 1600;
constexpr int32_t kHeBaselineLevel = 1800;
constexpr int32_t kLevelStep = 25;
constexpr float kLightScale = 0.60f;
constexpr float kMediumScale = 0.80f;
constexpr float kStrongScale = 1.00f;

const std::vector<std::string> kDurationPaths = {
        "/sys/class/leds/vibrator/duration",
        "/sys/class/leds/vibrator/oplus_duration",
};

const std::vector<std::string> kActivatePaths = {
        "/sys/class/leds/vibrator/activate",
        "/sys/class/leds/vibrator/oplus_activate",
};

const std::vector<std::string> kVmaxPaths = {
        "/sys/class/leds/vibrator/vmax",
};

struct EffectProfile {
    int32_t selector;
    int32_t intensity;
};

bool isSupportedEffect(Effect effect) {
    switch (effect) {
        case Effect::CLICK:
        case Effect::DOUBLE_CLICK:
        case Effect::TICK:
        case Effect::THUD:
        case Effect::POP:
        case Effect::HEAVY_CLICK:
        case Effect::TEXTURE_TICK:
            return true;
        default:
            return false;
    }
}

float getStrengthScale(EffectStrength strength) {
    switch (strength) {
        case EffectStrength::LIGHT:
            return kLightScale;
        case EffectStrength::MEDIUM:
            return kMediumScale;
        case EffectStrength::STRONG:
            return kStrongScale;
        default:
            return -1.0f;
    }
}

EffectProfile getEffectProfile(Effect effect) {
    switch (effect) {
        case Effect::TEXTURE_TICK:
            return {69, 55};
        case Effect::TICK:
            return {60, 78};
        case Effect::POP:
            return {5, 78};
        case Effect::CLICK:
        case Effect::DOUBLE_CLICK:
            return {5, 88};
        case Effect::HEAVY_CLICK:
        case Effect::THUD:
            return {5, 100};
        default:
            return {5, 88};
    }
}

}  // namespace

Vibrator::Vibrator() {
    mDurationPath = lookupPath(kDurationPaths);
    mActivatePath = lookupPath(kActivatePaths);
    mVmaxPath = lookupPath(kVmaxPaths);

    if (mDurationPath.empty() || mActivatePath.empty()) {
        ALOGE("LiveTap init failed: sysfs interface not found");
        return;
    }

    mF0 = LIVETAP_DEFAULT_F0;
    FILE* fp = fopen("/sys/class/leds/vibrator/f0", "r");
    if (fp != nullptr) {
        int val = 0;
        if (fscanf(fp, "%d", &val) == 1 && val >= 1000 && val <= 4000) {
            mF0 = val / 10.0f;
        }
        fclose(fp);
    }

    mReady = true;
    ALOGI("LiveTap sysfs init success: f0 %.1f Hz", mF0);
}

std::string Vibrator::lookupPath(const std::vector<std::string>& candidates) {
    std::string firstExisting;

    for (const auto& path : candidates) {
        if (access(path.c_str(), F_OK) != 0) {
            continue;
        }

        if (firstExisting.empty()) {
            firstExisting = path;
        }

        if (access(path.c_str(), W_OK) == 0) {
            return path;
        }
    }

    return firstExisting;
}

bool Vibrator::writeValue(const std::string& path, int32_t value) {
    int fd = open(path.c_str(), O_WRONLY | O_CLOEXEC);
    if (fd < 0) {
        ALOGE("LiveTap: failed to open %s: %s", path.c_str(), strerror(errno));
        return false;
    }

    char buf[16];
    int len = snprintf(buf, sizeof(buf), "%d", value);
    ssize_t ret = write(fd, buf, len);
    close(fd);

    if (ret != len) {
        ALOGE("LiveTap: failed to write %d to %s: %s", value, path.c_str(),
              ret < 0 ? strerror(errno) : "short write");
        return false;
    }

    return true;
}

ndk::ScopedAStatus Vibrator::getCapabilities(int32_t* _aidl_return) {
    *_aidl_return = mReady ? static_cast<int32_t>(IVibrator::CAP_ON_CALLBACK) : 0;
    if (mReady && !mVmaxPath.empty()) {
        *_aidl_return |= static_cast<int32_t>(IVibrator::CAP_AMPLITUDE_CONTROL);
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::off() {
    std::lock_guard<std::mutex> lock(mMutex);

    mTimedActive = false;
    mTimedLevel = kDefaultOnLevel;

    bool engineStopped = mEngine.stop();
    bool timedStopped = mActivatePath.empty() || writeValue(mActivatePath, 0);
    if (!engineStopped || !timedStopped) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_SERVICE_SPECIFIC));
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::on(int32_t timeoutMs,
                                const std::shared_ptr<IVibratorCallback>& callback) {
    if (!mReady) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_SERVICE_SPECIFIC));
    }

    if (timeoutMs <= 0) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_ARGUMENT));
    }

    int32_t duration;
    {
        std::lock_guard<std::mutex> lock(mMutex);

        if (!mEngine.stop()) {
            return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_SERVICE_SPECIFIC));
        }
        mTimedActive = false;
        if (!mVmaxPath.empty() && !writeValue(mVmaxPath, mTimedLevel)) {
            return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_SERVICE_SPECIFIC));
        }

        if (!writeValue(mDurationPath, timeoutMs) || !writeValue(mActivatePath, 1)) {
            return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_SERVICE_SPECIFIC));
        }

        duration = timeoutMs;
        mTimedActive = true;
    }

    if (callback != nullptr) {
        std::thread([callback, duration] {
            std::this_thread::sleep_for(std::chrono::milliseconds(duration));
            callback->onComplete();
        }).detach();
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::perform(Effect effect, EffectStrength es,
                                     const std::shared_ptr<IVibratorCallback>& callback,
                                     int32_t* _aidl_return) {
    if (!mEngine.isReady() || !isSupportedEffect(effect) || callback != nullptr) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }

    float strengthScale = getStrengthScale(es);
    if (strengthScale < 0.0f) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }

    std::lock_guard<std::mutex> lock(mMutex);
    mTimedActive = false;
    if (!mActivatePath.empty() && !writeValue(mActivatePath, 0)) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_SERVICE_SPECIFIC));
    }
    if (!mVmaxPath.empty() && !writeValue(mVmaxPath, kHeBaselineLevel)) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_SERVICE_SPECIFIC));
    }
    EffectProfile profile = getEffectProfile(effect);
    int32_t intensity = static_cast<int32_t>(std::round(profile.intensity * strengthScale));
    int32_t duration = mEngine.perform(profile.selector, intensity, effect == Effect::DOUBLE_CLICK);
    if (duration <= 0) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_SERVICE_SPECIFIC));
    }
    *_aidl_return = duration;

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::getSupportedEffects(std::vector<Effect>* _aidl_return) {
    if (!mEngine.isReady()) {
        _aidl_return->clear();
        return ndk::ScopedAStatus::ok();
    }
    *_aidl_return = {Effect::CLICK, Effect::DOUBLE_CLICK, Effect::TICK,        Effect::THUD,
                     Effect::POP,   Effect::HEAVY_CLICK,  Effect::TEXTURE_TICK};

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::setAmplitude(float amplitude) {
    if (!mReady) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_SERVICE_SPECIFIC));
    }

    if (!std::isfinite(amplitude) || amplitude <= 0.0f || amplitude > 1.0f) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_ARGUMENT));
    }
    if (mVmaxPath.empty()) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }

    int32_t steps = (kMaxLevel - kMinLevel) / kLevelStep;
    int32_t step = static_cast<int32_t>(std::round(amplitude * steps));
    int32_t level = kMinLevel + step * kLevelStep;

    std::lock_guard<std::mutex> lock(mMutex);
    if (mTimedActive && !writeValue(mVmaxPath, level)) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_SERVICE_SPECIFIC));
    }

    mTimedLevel = level;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::setExternalControl(bool /* enabled */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getCompositionDelayMax(int32_t* /* maxDelayMs */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getCompositionSizeMax(int32_t* /* maxSize */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getSupportedPrimitives(std::vector<CompositePrimitive>* supported) {
    if (supported != nullptr) {
        supported->clear();
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::getPrimitiveDuration(CompositePrimitive /* primitive */,
                                                  int32_t* /* durationMs */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::compose(const std::vector<CompositeEffect>& /* composite */,
                                     const std::shared_ptr<IVibratorCallback>& /* callback */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getSupportedAlwaysOnEffects(std::vector<Effect>* /* _aidl_return */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::alwaysOnEnable(int32_t /* id */, Effect /* effect */,
                                            EffectStrength /* strength */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::alwaysOnDisable(int32_t /* id */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getResonantFrequency(float* resonantFreqHz) {
    if (resonantFreqHz == nullptr) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_ARGUMENT));
    }
    *resonantFreqHz = static_cast<float>(mF0 > 0 ? mF0 : LIVETAP_DEFAULT_F0);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::getQFactor(float* qFactor) {
    if (qFactor == nullptr) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_ARGUMENT));
    }
    *qFactor = 10.0f;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::getFrequencyResolution(float* /* freqResolutionHz */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getFrequencyMinimum(float* /* freqMinimumHz */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getBandwidthAmplitudeMap(std::vector<float>* /* _aidl_return */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getPwlePrimitiveDurationMax(int32_t* /* durationMs */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getPwleCompositionSizeMax(int32_t* /* maxSize */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getSupportedBraking(std::vector<Braking>* /* supported */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::composePwle(const std::vector<PrimitivePwle>& /* composite */,
                                         const std::shared_ptr<IVibratorCallback>& /* callback */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

}  // namespace vibrator
}  // namespace hardware
}  // namespace android
}  // namespace aidl
