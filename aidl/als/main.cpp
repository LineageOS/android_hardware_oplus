/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "AreaCapture.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using ::aidl::vendor::lineage::oplus_als::AreaCapture;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    ABinderProcess_startThreadPool();
    std::shared_ptr<AreaCapture> ac = ndk::SharedRefBase::make<AreaCapture>();

    const auto instance = std::string(AreaCapture::descriptor) + "/default";
    binder_status_t status = AServiceManager_addService(ac->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
