/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.oplus"

#include "GloveMode.h"

#include <android-base/file.h>
#include <android-base/logging.h>

#include <OplusTouchConstants.h>

using ::android::base::ReadFileToString;
using ::android::base::WriteStringToFile;

namespace {

constexpr const char* kGloveModeEnablePath = "/proc/touchpanel/glove_mode_enable";

}  // anonymous namespace

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

GloveMode::GloveMode(std::shared_ptr<IOplusTouch> oplusTouch)
    : mOplusTouch(std::move(oplusTouch)) {}

ndk::ScopedAStatus GloveMode::getEnabled(bool* _aidl_return) {
    std::string value;

    if (mOplusTouch) {
        mOplusTouch->touchReadNodeFile(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                       OplusTouchConstants::GLOVE_MODE_ENABLE_NODE, &value);
    } else if (!ReadFileToString(kGloveModeEnablePath, &value)) {
        LOG(ERROR) << "Failed to read current GloveMode state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = value[0] != '0';
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus GloveMode::setEnabled(bool enable) {
    if (mOplusTouch) {
        mOplusTouch->touchWriteNodeFileOneWay(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                              OplusTouchConstants::GLOVE_MODE_ENABLE_NODE,
                                              enable ? "1" : "0");
    } else if (!WriteStringToFile(enable ? "1" : "0", kGloveModeEnablePath, true)) {
        LOG(ERROR) << "Failed to write GloveMode state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
