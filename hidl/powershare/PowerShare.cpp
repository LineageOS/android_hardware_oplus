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

#define LOG_TAG "vendor.lineage.powershare@1.0-service.oplus"

#include "PowerShare.h"

#include <android-base/file.h>

using ::android::base::ReadFileToString;
using ::android::base::WriteStringToFile;

namespace {

constexpr const char* kWirelessTxEnablePath = "/proc/wireless/enable_tx";

}  // anonymous namespace

namespace vendor {
namespace lineage {
namespace powershare {
namespace V1_0 {
namespace implementation {

Return<bool> PowerShare::isEnabled() {
    std::string value;
    return ReadFileToString(kWirelessTxEnablePath, &value) && value != "disable\n";
}

Return<bool> PowerShare::setEnabled(bool enable) {
    return WriteStringToFile(enable ? "1" : "0", kWirelessTxEnablePath, true);
}

Return<uint32_t> PowerShare::getMinBattery() {
    return 0;
}

Return<uint32_t> PowerShare::setMinBattery(uint32_t /*minBattery*/) {
    return getMinBattery();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace powershare
}  // namespace lineage
}  // namespace vendor
