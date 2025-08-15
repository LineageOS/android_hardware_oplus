/*
 * SPDX-FileCopyrightText: 2019-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/livedisplay/BnDisplayModes.h>
#include <livedisplay/sdm/SDMController.h>
#include <map>

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {

class DisplayModes : public BnDisplayModes {
  public:
    DisplayModes(std::shared_ptr<sdm::SDMController> controller);

    using DisplayModeSetCallback = std::function<void()>;
    inline void registerDisplayModeSetCallback(DisplayModeSetCallback callback) {
        mOnDisplayModeSet = callback;
    }

    // Methods from ::aidl::vendor::lineage::livedisplay::BnDisplayModes follow.
    ndk::ScopedAStatus getDisplayModes(std::vector<DisplayMode>* _aidl_return) override;
    ndk::ScopedAStatus getCurrentDisplayMode(DisplayMode* _aidl_return) override;
    ndk::ScopedAStatus getDefaultDisplayMode(DisplayMode* _aidl_return) override;
    ndk::ScopedAStatus setDisplayMode(int32_t modeID, bool makeDefault) override;

  private:
    struct ModeInfo {
        std::string name;
        int32_t displayModeId;
        uint32_t seedMode;
    };
    static const std::map<int32_t, ModeInfo> kModeMap;
    std::shared_ptr<sdm::SDMController> mController;
    int32_t mOplusDisplayFd;
    int32_t mCurrentModeId;
    int32_t mDefaultModeId;
    DisplayModeSetCallback mOnDisplayModeSet;
};

}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
