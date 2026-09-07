/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <array>
#include <cstdint>

namespace aidl::android::hardware::vibrator {

class LiveTapEngine {
  public:
    LiveTapEngine();
    bool isReady() const { return mReady; }
    bool stop();
    int32_t perform(int32_t selector, int32_t intensity, bool doubleClick);

  private:
    static constexpr std::array<int32_t, 5> kSelectors = {0, 5, 41, 60, 69};
    std::array<int32_t, kSelectors.size()> mDurationsMs = {};
    bool mInitialized = false;
    bool mReady = false;
    int (*mOff)() = nullptr;
    int (*mPost)(int32_t*, int, int, int, int, int) = nullptr;
};

}  // namespace aidl::android::hardware::vibrator
