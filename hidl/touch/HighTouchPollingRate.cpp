/*
 * Copyright (C) 2022 The LineageOS Project
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

#define LOG_TAG "vendor.lineage.touch@1.0-service.oplus"

#include "HighTouchPollingRate.h"

#include <android-base/file.h>

#ifdef ENABLE_OPLUSTOUCH
#include <android/binder_manager.h>
#endif

#ifndef ENABLE_OPLUSTOUCH
using ::android::base::ReadFileToString;
using ::android::base::WriteStringToFile;

namespace {

constexpr const char* kGameSwitchEnablePath = "/proc/touchpanel/game_switch_enable";

}  // anonymous namespace
#endif

namespace vendor {
namespace lineage {
namespace touch {
namespace V1_0 {
namespace implementation {

Return<bool> HighTouchPollingRate::isEnabled() {
    std::string value;
#ifdef ENABLE_OPLUSTOUCH
    // Connect to IOplusTouch
    const std::string instance = std::string() + IOplusTouch::descriptor + "/default";
    std::shared_ptr<IOplusTouch> oplusTouch = IOplusTouch::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(instance.c_str())));

    // Read current value
    oplusTouch->touchReadNodeFile(0, 26, &value);

    return value[0] != '0';
#else
    return ReadFileToString(kGameSwitchEnablePath, &value) && value[0] != '0';
#endif
}

Return<bool> HighTouchPollingRate::setEnabled(bool enabled) {
#ifdef ENABLE_OPLUSTOUCH
    int result;

    // Connect to IOplusTouch
    const std::string instance = std::string() + IOplusTouch::descriptor + "/default";
    std::shared_ptr<IOplusTouch> oplusTouch = IOplusTouch::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(instance.c_str())));

    // Always keep gestures enabled
    oplusTouch->touchWriteNodeFile(0, 26, enabled ? "1" : "0", &result);

    return true;
#else
    return WriteStringToFile(enabled ? "1" : "0", kGameSwitchEnablePath, true);
#endif
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
