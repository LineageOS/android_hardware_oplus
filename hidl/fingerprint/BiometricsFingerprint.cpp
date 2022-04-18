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

#define LOG_TAG "android.hardware.biometrics.fingerprint@2.3-service.oplus"

#include <android-base/file.h>

#include "BiometricsFingerprint.h"
#include "BiometricsFingerprintClientCallbackEx.h"

using ::android::base::ReadFileToString;
using ::android::base::WriteStringToFile;

namespace {

constexpr const char* kDimlayerHbmPath = DIMLAYER_HBM_PATH;
constexpr const char* kNotifyFpPressPath = "/sys/kernel/oplus_display/notify_fppress";

}  // anonymous namespace

namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {
namespace V2_3 {
namespace implementation {

BiometricsFingerprint::BiometricsFingerprint() {
    mOplusBiometricsFingerprint = IOplusBiometricsFingerprint::getService();
    mOplusBiometricsFingerprintCallback = new BiometricsFingerprintClientCallbackEx();

    mOplusBiometricsFingerprint->setHalCallback(mOplusBiometricsFingerprintCallback);
}

Return<uint64_t> BiometricsFingerprint::setNotify(
        const sp<IBiometricsFingerprintClientCallback>& clientCallback) {
    return mOplusBiometricsFingerprint->setNotify(std::move(clientCallback));
}

Return<uint64_t> BiometricsFingerprint::preEnroll() {
    return mOplusBiometricsFingerprint->preEnroll();
}

Return<RequestStatus> BiometricsFingerprint::enroll(const hidl_array<uint8_t, 69>& hat,
                                                    uint32_t gid, uint32_t timeoutSec) {
    return mOplusBiometricsFingerprint->enroll(hat, gid, timeoutSec);
}

Return<RequestStatus> BiometricsFingerprint::postEnroll() {
    return mOplusBiometricsFingerprint->postEnroll();
}

Return<uint64_t> BiometricsFingerprint::getAuthenticatorId() {
    return mOplusBiometricsFingerprint->getAuthenticatorId();
}

Return<RequestStatus> BiometricsFingerprint::cancel() {
    return mOplusBiometricsFingerprint->cancel();
}

Return<RequestStatus> BiometricsFingerprint::enumerate() {
    return mOplusBiometricsFingerprint->enumerate();
}

Return<RequestStatus> BiometricsFingerprint::remove(uint32_t gid, uint32_t fid) {
    return mOplusBiometricsFingerprint->remove(gid, fid);
}

Return<RequestStatus> BiometricsFingerprint::setActiveGroup(uint32_t gid,
                                                            const hidl_string& storePath) {
    return mOplusBiometricsFingerprint->setActiveGroup(gid, storePath);
}

Return<RequestStatus> BiometricsFingerprint::authenticate(uint64_t operationId, uint32_t gid) {
    WriteStringToFile("0", kDimlayerHbmPath, true);
    WriteStringToFile("0", kNotifyFpPressPath, true);
    return mOplusBiometricsFingerprint->authenticate(operationId, gid);
}

Return<bool> BiometricsFingerprint::isUdfps(uint32_t sensorID) {
    return mOplusBiometricsFingerprint->isUdfps(sensorID);
}

Return<void> BiometricsFingerprint::onFingerDown(uint32_t x, uint32_t y, float minor, float major) {
    WriteStringToFile("1", kDimlayerHbmPath, true);
    WriteStringToFile("1", kNotifyFpPressPath, true);
    return mOplusBiometricsFingerprint->onFingerDown(x, y, minor, major);
}

Return<void> BiometricsFingerprint::onFingerUp() {
    WriteStringToFile("0", kDimlayerHbmPath, true);
    WriteStringToFile("0", kNotifyFpPressPath, true);
    return mOplusBiometricsFingerprint->onFingerUp();
}

}  // namespace implementation
}  // namespace V2_3
}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
