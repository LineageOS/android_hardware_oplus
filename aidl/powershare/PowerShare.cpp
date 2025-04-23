/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.powershare-service.oplus"

#include "PowerShare.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

using ::android::base::ReadFileToString;
using ::android::base::WriteStringToFile;

namespace {

constexpr const char* kWirelessTxEnablePath = "/proc/wireless/enable_tx";

}  // anonymous namespace

namespace aidl::vendor::lineage::powershare {

ndk::ScopedAStatus PowerShare::getMinBattery(int32_t* _aidl_return) {
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PowerShare::isEnabled(bool* _aidl_return) {
    std::string value;
    if (!ReadFileToString(kWirelessTxEnablePath, &value)) {
        LOG(ERROR) << "Failed to read current PowerShare state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = value != "disable\n";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PowerShare::setEnabled(bool enable, bool* _aidl_return) {
    if (!WriteStringToFile(enable ? "1" : "0", kWirelessTxEnablePath, true)) {
        LOG(ERROR) << "Failed to write PowerShare state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = enable;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PowerShare::setMinBattery(int32_t minBattery, int32_t* _aidl_return) {
    *_aidl_return = minBattery;
    return ndk::ScopedAStatus::ok();
}

}  // namespace aidl::vendor::lineage::powershare
