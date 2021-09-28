/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <livedisplay/oplus/AdaptiveBacklight.h>
#include <oplus/oplus_display_panel.h>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_1 {
namespace implementation {

AdaptiveBacklight::AdaptiveBacklight() : mOplusDisplayFd(open("/dev/oplus_display", O_RDWR)) {}

Return<bool> AdaptiveBacklight::isEnabled() {
    unsigned int value;
    return ioctl(mOplusDisplayFd, PANEL_IOCTL_GET_CABC_STATUS, &value) == 0 && value > 0;
}

Return<bool> AdaptiveBacklight::setEnabled(bool enabled) {
    unsigned int value = enabled;
    return isEnabled() == enabled ||
           ioctl(mOplusDisplayFd, PANEL_IOCTL_SET_CABC_STATUS, &value) == 0;
}

}  // namespace implementation
}  // namespace V2_1
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
