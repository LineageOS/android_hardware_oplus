/*
 * Copyright (C) 2022-2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

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
    *_aidl_return = ioctl(mOplusDisplayFd, PANEL_IOCTL_GET_HBM, &value) == 0 && value > 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus SunlightEnhancement::setEnabled(bool enabled) {
    bool isEnabled;
    getEnabled(&isEnabled);

    if (isEnabled != enabled) {
        unsigned int value = enabled;
        ioctl(mOplusDisplayFd, PANEL_IOCTL_SET_HBM, &value);
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
