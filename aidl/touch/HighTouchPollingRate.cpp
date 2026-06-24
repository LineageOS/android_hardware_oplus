/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.oplus"

#include "HighTouchPollingRate.h"

#include <android-base/file.h>
#include <android-base/logging.h>

#include <OplusTouchConstants.h>

using ::android::base::ReadFileToString;
using ::android::base::WriteStringToFile;

namespace {

constexpr const char* kGameSwitchEnablePath = "/proc/touchpanel/game_switch_enable";

}  // anonymous namespace

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

HighTouchPollingRate::HighTouchPollingRate(std::shared_ptr<IOplusTouch> oplusTouch)
    : mOplusTouch(std::move(oplusTouch)), mHighestSupportedMode(highestSupportedMode()) {}

ndk::ScopedAStatus HighTouchPollingRate::getEnabled(bool* _aidl_return) {
    std::string value;

    if (mOplusTouch) {
        mOplusTouch->touchReadNodeFile(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                       OplusTouchConstants::GAME_SWITCH_ENABLE_NODE, &value);
    } else if (!ReadFileToString(kGameSwitchEnablePath, &value)) {
        LOG(ERROR) << "Failed to read current HighTouchPollingRate state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = value[0] != '0';
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus HighTouchPollingRate::setEnabled(bool enable) {
    if (mOplusTouch) {
        int aidl_return = 0;
        mOplusTouch->touchWriteNodeFile(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                        OplusTouchConstants::GAME_SWITCH_ENABLE_NODE,
                                        enable ? "1" : "0", &aidl_return);
    } else if (!WriteStringToFile(enable ? "1" : "0", kGameSwitchEnablePath, true)) {
        LOG(ERROR) << "Failed to write HighTouchPollingRate state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    return ndk::ScopedAStatus::ok();
}

std::string HighTouchPollingRate::highestSupportedMode() {
    if (mOplusTouch) {
        int aidl_return = 0;

        // Backup current mode
        std::string backup;

        mOplusTouch->touchReadNodeFile(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                       OplusTouchConstants::GAME_SWITCH_ENABLE_NODE, &backup);

        // Find highest supported game switch mode
        std::string ret(backup);

        for (int i = 20; i > 0; i--) {
            std::string value = std::to_string(i);

            mOplusTouch->touchWriteNodeFile(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                            OplusTouchConstants::GAME_SWITCH_ENABLE_NODE, value,
                                            &aidl_return);
            mOplusTouch->touchReadNodeFile(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                           OplusTouchConstants::GAME_SWITCH_ENABLE_NODE, &ret);

            if (value == ret) {
                LOG(INFO) << "HighTouchPollingRate highest supported mode = " << value;
                break;
            }
        }

        // Backup previous mode
        mOplusTouch->touchWriteNodeFile(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                        OplusTouchConstants::GAME_SWITCH_ENABLE_NODE, backup,
                                        &aidl_return);

        return ret;
    } else {
        return "1";
    }
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
