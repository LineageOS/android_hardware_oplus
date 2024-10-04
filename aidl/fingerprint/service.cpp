/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Fingerprint.h"

#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <android-base/logging.h>

using ::aidl::android::hardware::biometrics::fingerprint::Fingerprint;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<Fingerprint> fingerprint = ndk::SharedRefBase::make<Fingerprint>();

    const std::string instance = std::string() + Fingerprint::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(fingerprint->asBinder().get(), instance.c_str());
    CHECK(status == STATUS_OK);

    const std::string instance = std::string() + Fingerprint::descriptor + "/oneplus";
    SpAIBinder vibBinder = SpAIBinder(AServiceManager_getService(kInstance.c_str()));
    std::shared_ptr<IFingerprint> fp = IFingerprint::fromBinder(vibBinder);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE; // should not reach
}
