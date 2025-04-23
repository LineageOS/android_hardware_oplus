/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.powershare-service.oplus"

#include "PowerShare.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using aidl::vendor::lineage::powershare::PowerShare;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<PowerShare> service = ndk::SharedRefBase::make<PowerShare>();

    const std::string instance = std::string(PowerShare::descriptor) + "/default";
    binder_status_t status =
            AServiceManager_addService(service->asBinder().get(), instance.c_str());

    if (status != STATUS_OK) {
        LOG(ERROR) << "Can't register IPowerShare/default";
        return 1;
    }

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
