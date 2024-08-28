/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <livedisplay/sdm/SDMController.h>
#include <vendor/lineage/livedisplay/2.1/IDisplayModes.h>
#include <map>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_1 {
namespace implementation {

using ::android::sp;
using ::android::hardware::Return;
using ::android::hardware::Void;

class DisplayModes : public IDisplayModes {
  public:
    DisplayModes(std::shared_ptr<V2_0::sdm::SDMController> controller);

    using DisplayModeSetCallback = std::function<void()>;
    inline void registerDisplayModeSetCallback(DisplayModeSetCallback callback) {
        mOnDisplayModeSet = callback;
    }

    // Methods from ::vendor::lineage::livedisplay::V2_1::IDisplayModes follow.
    Return<void> getDisplayModes(getDisplayModes_cb resultCb) override;
    Return<void> getCurrentDisplayMode(getCurrentDisplayMode_cb resultCb) override;
    Return<void> getDefaultDisplayMode(getDefaultDisplayMode_cb ResultCb) override;
    Return<bool> setDisplayMode(int32_t modeID, bool makeDefault) override;

  private:
    struct ModeInfo {
        std::string name;
        int32_t displayModeId;
        uint32_t seedMode;
    };
    static const std::map<int32_t, ModeInfo> kModeMap;
    std::shared_ptr<V2_0::sdm::SDMController> mController;
    int32_t mOplusDisplayFd;
    int32_t mCurrentModeId;
    int32_t mDefaultModeId;
    DisplayModeSetCallback mOnDisplayModeSet;
};

}  // namespace implementation
}  // namespace V2_1
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
