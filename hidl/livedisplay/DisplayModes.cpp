/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "DisplayModesService"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <fcntl.h>
#include <livedisplay/oplus/DisplayModes.h>
#include <oplus/oplus_display_panel.h>
#include <fstream>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_1 {
namespace implementation {

static const std::string kModeBasePath = "/sys/class/drm/card0-DSI-1/";
static const std::string kDefaultPath = "/data/vendor/display/default_display_mode";

// Mode ids here must match qdcm display mode ids
const std::map<int32_t, DisplayModes::ModeInfo> DisplayModes::kModeMap = {
        {0, {"Vivid", 0, 0}},
        {1, {"Natural", 1, 1}},
        {2, {"Cinematic", 0, 1}},
        {3, {"Brilliant", 4, 0}},
};

DisplayModes::DisplayModes(std::shared_ptr<V2_0::sdm::SDMController> controller)
    : mController(std::move(controller)),
      mOplusDisplayFd(open("/dev/oplus_display", O_RDWR)),
      mCurrentModeId(0),
      mDefaultModeId(0) {
    std::ifstream defaultFile(kDefaultPath);

    defaultFile >> mDefaultModeId;
    LOG(DEBUG) << "Default file read result " << mDefaultModeId << " fail " << defaultFile.fail();

    setDisplayMode(mDefaultModeId, false);
}

// Methods from ::vendor::lineage::livedisplay::V2_1::IDisplayModes follow.
Return<void> DisplayModes::getDisplayModes(getDisplayModes_cb resultCb) {
    std::vector<V2_0::DisplayMode> modes;

    for (const auto& entry : kModeMap) {
        modes.push_back({entry.first, entry.second.name});
    }
    resultCb(modes);
    return Void();
}

Return<void> DisplayModes::getCurrentDisplayMode(getCurrentDisplayMode_cb resultCb) {
    resultCb({mCurrentModeId, kModeMap.at(mCurrentModeId).name});
    return Void();
}

Return<void> DisplayModes::getDefaultDisplayMode(getDefaultDisplayMode_cb resultCb) {
    resultCb({mDefaultModeId, kModeMap.at(mDefaultModeId).name});
    return Void();
}

Return<bool> DisplayModes::setDisplayMode(int32_t modeID, bool makeDefault) {
    const auto iter = kModeMap.find(modeID);
    if (iter == kModeMap.end()) {
        return false;
    }
    if (mOplusDisplayFd >= 0) {
        ioctl(mOplusDisplayFd, PANEL_IOCTL_SET_SEED, &iter->second.seedMode);
    }
    mController->setActiveDisplayMode(iter->second.displayModeId);
    mCurrentModeId = iter->first;
    if (makeDefault) {
        std::ofstream defaultFile(kDefaultPath);
        defaultFile << iter->first;
        if (!defaultFile.fail()) {
            mController->setDefaultDisplayMode(iter->second.displayModeId);
            mDefaultModeId = iter->first;
        }
    }
    if (mOnDisplayModeSet) {
        mOnDisplayModeSet();
    }
    return true;
}

}  // namespace implementation
}  // namespace V2_1
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
