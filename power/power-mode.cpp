/*
 * SPDX-FileCopyrightText: 2024-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <aidl/android/hardware/power/BnPower.h>
#include <aidl/vendor/oplus/hardware/touch/IOplusTouch.h>

#include <android-base/logging.h>
#include <android/binder_manager.h>

#include <OplusTouchConstants.h>

namespace aidl {
namespace google {
namespace hardware {
namespace power {
namespace impl {
namespace pixel {

using aidl::android::hardware::power::Mode;
using aidl::vendor::oplus::hardware::touch::IOplusTouch;

bool isDeviceSpecificModeSupported(Mode type, bool* _aidl_return) {
    switch (type) {
        case Mode::DOUBLE_TAP_TO_WAKE:
            *_aidl_return = true;
            return true;
        default:
            return false;
    }
}

bool setDeviceSpecificMode(Mode type, bool enabled) {
    switch (type) {
        case Mode::DOUBLE_TAP_TO_WAKE: {
            int result = 0;

            const std::string instance = std::string() + IOplusTouch::descriptor + "/default";
            std::shared_ptr<IOplusTouch> oplusTouch = IOplusTouch::fromBinder(
                    ndk::SpAIBinder(AServiceManager_waitForService(instance.c_str())));
            LOG(INFO) << "Power mode: " << toString(type) << " isDoubleTapEnabled: " << enabled;

            // For normal devices
            oplusTouch->touchWriteNodeFile(DEFAULT_TP_IC_ID, DOUBLE_TAP_ENABLE_NODE,
                                           enabled ? "1" : "0", &result);
            LOG(DEBUG) << "Power-Touch write double_tap_enable result: " << result;

            // For tcm hbp driver devices
            oplusTouch->touchWriteNodeFile(
                    DEFAULT_TP_IC_ID, DOUBLE_TAP_INDEP_NODE,
                    std::to_string(enabled ? DOUBLE_TAP_INDEP_ENABLED : DOUBLE_TAP_INDEP_DISABLED),
                    &result);
            LOG(DEBUG) << "Power-Touch write double_tap_enable_indep result: " << result;
            return true;
        }
        default:
            return false;
    }
}

}  // namespace pixel
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace google
}  // namespace aidl
