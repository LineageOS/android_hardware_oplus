/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include <thread>

#include <aidl/vendor/oplus/hardware/radio/IOplusRadio.h>

#include "OplusRadioResponse.h"

using aidl::vendor::oplus::hardware::radio::IOplusRadio;
using aidl::vendor::oplus::hardware::radio::OplusRadioResponse;
using android::base::GetIntProperty;
using std::chrono_literals::operator""s;

namespace {

enum OplusNrMode {
    NSA_PRE,
    NSA_ONLY,
    SA_ONLY,
    SA_PRE,
    NR_MODE_MAX,
};
constexpr auto kDefaultAutoNrMode = SA_PRE;
constexpr auto kPropertyDefaultAutoNrMode = "ro.vendor.radio.auto_nr_mode";

constexpr auto kIRadioAidlServiceName = "vendor.oplus.hardware.radio.IRadioStable/OplusRadio";
constexpr auto kMaxSimSlot = 2;
constexpr auto kOplusRilSerial = 1001;

OplusNrMode GetDefaultAutoNrMode() {
    auto mode = GetIntProperty(kPropertyDefaultAutoNrMode, static_cast<int>(kDefaultAutoNrMode));
    if (mode < NSA_PRE || mode >= NR_MODE_MAX) {
        LOG(WARNING) << "Invalid NR mode: " << mode << ", using default: " << kDefaultAutoNrMode;
        return kDefaultAutoNrMode;
    }
    return static_cast<OplusNrMode>(mode);
}

};  // anonymous namespace

int main() {
    int rc = EXIT_SUCCESS;

    ABinderProcess_setThreadPoolMaxThreadCount(1);
    ABinderProcess_startThreadPool();

    auto mode = GetDefaultAutoNrMode();
    for (auto i = 0; i < kMaxSimSlot; ++i) {
        auto instance = kIRadioAidlServiceName + std::to_string(i);
        if (!AServiceManager_isDeclared(instance.c_str())) {
            // Instance not declared in manifest, skip.
            continue;
        }
        auto radio = IOplusRadio::fromBinder(
                ndk::SpAIBinder(AServiceManager_waitForService(instance.c_str())));
        if (!radio) {
            LOG(DEBUG) << "Failed to get service " << instance;
            continue;
        }

        int32_t result = 0;
        auto resp_cb = ndk::SharedRefBase::make<OplusRadioResponse>(result);
        radio->setCallback(resp_cb, nullptr);

        bool succeed = false;
        for (auto retry = 100; retry > 0; --retry) {
            auto status = radio->setNrMode(kOplusRilSerial, mode);
            if (!status.isOk() || result != 0) {
                LOG(ERROR) << "setNrMode failed for SIM" << i
                           << ", (remaining retries: " << retry - 1 << ")";
                std::this_thread::sleep_for(1s);
                continue;
            }
            succeed = true;
            break;
        }
        if (!succeed) rc = EXIT_FAILURE;
    }

    return rc;
}
