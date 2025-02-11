/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ScreenSampler.h"

#include <cstdlib>
#include <memory>
#include <string>

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using aidl::vendor::lineage::oplus_fusionlight::ScreenSampler;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(2);
    ABinderProcess_startThreadPool();

    const std::shared_ptr<ScreenSampler> service = ndk::SharedRefBase::make<ScreenSampler>();
    const std::string instance = std::string(ScreenSampler::descriptor) + "/default";
    const binder_status_t status =
            AServiceManager_addService(service->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK) << "Failed to register " << instance;
    LOG(INFO) << "Registered " << instance;

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;
}
