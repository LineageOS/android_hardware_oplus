/*
 * SPDX-FileCopyrightText: 2022-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "SunlightEnhancementService"

#include <android-base/logging.h>
#include <fcntl.h>
#include <livedisplay/oplus/SunlightEnhancement.h>
#include <oplus/oplus_display_panel.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {

SunlightEnhancement::SunlightEnhancement() : mOplusDisplayFd(open("/dev/oplus_display", O_RDWR)) {}

ndk::ScopedAStatus SunlightEnhancement::getEnabled(bool* _aidl_return) {
    unsigned int value;
    if (ioctl(mOplusDisplayFd, PANEL_IOCTL_GET_HBM, &value) != 0) {
        LOG(ERROR) << "Failed to read current SunlightEnhancement state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
    *_aidl_return = value > 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus SunlightEnhancement::setEnabled(bool enabled) {
    bool isEnabled;
    if (auto status = getEnabled(&isEnabled); !status.isOk()) {
        return status;
    }
    unsigned int value = enabled;
    if (isEnabled != enabled && ioctl(mOplusDisplayFd, PANEL_IOCTL_SET_HBM, &value) != 0) {
        LOG(ERROR) << "Failed to set SunlightEnhancement state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
    return ndk::ScopedAStatus::ok();
}

}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
