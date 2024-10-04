/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Fingerprint.h"

namespace aidl::android::hardware::biometrics::fingerprint {

Fingerprint::Fingerprint() {
    const std::string instance = std::string() + Fingerprint::descriptor + "/oplus";
    ndk::SpAIBinder fingerprintBinder = SpAIBinder(AServiceManager_getService(instance.c_str()));
    mOplusFingerprint = IFingerprint::fromBinder(fingerprintBinder);
}

ndk::ScopedAStatus Fingerprint::getSensorProps(std::vector<SensorProps>* out) {
    return mOplusFingerprint->getSensorProps(out);
}

ndk::ScopedAStatus Fingerprint::createSession(int32_t sensorId, int32_t userId,
                                              const std::shared_ptr<ISessionCallback>& cb,
                                              std::shared_ptr<ISession>* out) {
    return mOplusFingerprint->createSession(sensorId, userId, cb, out);
}

}  // namespace aidl::android::hardware::biometrics::fingerprint
