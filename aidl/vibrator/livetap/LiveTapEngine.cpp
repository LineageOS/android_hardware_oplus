/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "LiveTapEngine.h"

#include <dlfcn.h>
#include <log/log.h>

#include <algorithm>
#include <cstring>

namespace aidl::android::hardware::vibrator {

namespace {

constexpr int32_t kDoubleClickGapMs = 100;

int32_t readDuration(const void* descriptor) {
    if (descriptor == nullptr) return -1;
    // The exported getter returns &data_ptr, not the wave header itself.
    const void* payload = nullptr;
    std::memcpy(&payload, descriptor, sizeof(payload));
    if (payload == nullptr) return -1;
    std::array<uint32_t, 2> header;
    std::memcpy(header.data(), payload, sizeof(header));
    constexpr std::array<uint32_t, 3> kSampleRates = {12000, 24000, 48000};
    if (header[0] == 0 || header[0] > 2000 || header[1] >= kSampleRates.size()) return -1;
    uint32_t rate = kSampleRates[header[1]];
    return static_cast<int32_t>((header[0] * 1000 + rate - 1) / rate);
}

}  // namespace

LiveTapEngine::LiveTapEngine() {
    void* library = dlopen("libsivibrator.so", RTLD_NOW | RTLD_LOCAL);
    if (library == nullptr) {
        ALOGE("LiveTap: cannot load stock engine: %s", dlerror());
        return;
    }

    auto init = reinterpret_cast<int (*)(int*)>(dlsym(library, "si_vibra_init"));
    mOff = reinterpret_cast<decltype(mOff)>(dlsym(library, "si_vibra_off"));
    mPost = reinterpret_cast<decltype(mPost)>(dlsym(library, "si_vibra_looper_post"));
    auto getWave = reinterpret_cast<const void* (*)(int, int)>(
            dlsym(library, "get_he_short_vib_prefabed"));
    if (init == nullptr || mOff == nullptr || mPost == nullptr || getWave == nullptr) {
        ALOGE("LiveTap: stock engine ABI is incomplete");
        dlclose(library);
        return;
    }

    int ignoredStatus = 0;
    int status = init(&ignoredStatus);
    if (status != 0) {
        ALOGE("LiveTap: stock engine init failed: %d", status);
        return;
    }
    mInitialized = true;
    for (size_t i = 0; i < kSelectors.size(); ++i) {
        mDurationsMs[i] = readDuration(getWave(0, kSelectors[i]));
        if (mDurationsMs[i] <= 0) {
            ALOGE("LiveTap: stock wave unavailable for selector %d", kSelectors[i]);
            return;
        }
    }
    mReady = true;
}

bool LiveTapEngine::stop() {
    if (!mInitialized) return true;
    int status = mOff();
    if (status != 0) {
        ALOGE("LiveTap: stock engine stop failed: %d", status);
        return false;
    }
    return true;
}

int32_t LiveTapEngine::perform(int32_t selector, int32_t intensity, bool doubleClick) {
    if (!mReady || intensity < 1 || intensity > 100) return -1;
    auto selected = std::find(kSelectors.begin(), kSelectors.end(), selector);
    if (selected == kSelectors.end()) return -1;
    int32_t duration = mDurationsMs[selected - kSelectors.begin()];
    std::array<int32_t, 35> pattern = {};
    pattern[0] = 1;
    pattern[1] = 0x1001;
    pattern[3] = intensity;
    pattern[4] = selector;
    int words = 18;
    if (doubleClick) {
        pattern[18] = 0x1001;
        pattern[19] = duration + kDoubleClickGapMs;
        pattern[20] = intensity;
        pattern[21] = selector;
        words = 35;
    }
    int status = mPost(pattern.data(), words, 0, 1, 255, 0);
    if (status <= 0) {
        ALOGE("LiveTap: stock engine rejected HE sequence: %d", status);
        return -1;
    }
    return doubleClick ? 2 * duration + kDoubleClickGapMs : duration;
}

}  // namespace aidl::android::hardware::vibrator
