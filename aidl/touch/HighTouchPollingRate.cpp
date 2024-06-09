/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.oplus"

#include "HighTouchPollingRate.h"

#include <android-base/file.h>
#include <android-base/logging.h>

#ifdef USE_OPLUSTOUCH
#include <android/binder_manager.h>
#else
using ::android::base::ReadFileToString;
using ::android::base::WriteStringToFile;

namespace {

constexpr const char* kGameSwitchEnablePath = "/proc/touchpanel/game_switch_enable";

}  // anonymous namespace
#endif

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

ndk::ScopedAStatus HighTouchPollingRate::getEnabled(bool* _aidl_return) {
    std::string value;

#ifdef USE_OPLUSTOUCH
    // Connect to IOplusTouch
    const std::string instance = std::string() + IOplusTouch::descriptor + "/default";
    std::shared_ptr<IOplusTouch> oplusTouch = IOplusTouch::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(instance.c_str())));

    oplusTouch->touchReadNodeFile(0, 26, &value);
#else
    if (!ReadFileToString(kGameSwitchEnablePath, &value)) {
        LOG(ERROR) << "Failed to read current HighTouchPollingRate state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
#endif

    *_aidl_return = value[0] != '0';
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus HighTouchPollingRate::setEnabled(bool enable) {
#ifdef USE_OPLUSTOUCH
    int result;

    // Connect to IOplusTouch
    const std::string instance = std::string() + IOplusTouch::descriptor + "/default";
    std::shared_ptr<IOplusTouch> oplusTouch = IOplusTouch::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(instance.c_str())));

    oplusTouch->touchWriteNodeFile(0, 26, enable ? "1" : "0", &result);
#else
    if (!WriteStringToFile(enable ? "1" : "0", kGameSwitchEnablePath, true)) {
        LOG(ERROR) << "Failed to write HighTouchPollingRate state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
#endif

    return ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
