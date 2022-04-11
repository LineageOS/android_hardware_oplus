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

#include "BiometricsFingerprintClientCallbackEx.h"

namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {
namespace V2_3 {
namespace implementation {

Return<void> BiometricsFingerprintClientCallbackEx::onEngineeringInfoUpdated(
        uint32_t, const hidl_vec<uint32_t>&, const hidl_vec<hidl_string>&) {
    return Void();
}

Return<void> BiometricsFingerprintClientCallbackEx::onFingerprintCmd(int32_t,
                                                                     const hidl_vec<uint32_t>&,
                                                                     uint32_t) {
    return Void();
}

}  // namespace implementation
}  // namespace V2_3
}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
