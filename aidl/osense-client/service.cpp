/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "OsenseAidlHalReporter.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using ::aidl::vendor::oplus::hardware::osense::client::OsenseAidlHalReporter;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<OsenseAidlHalReporter> reporter =
            ndk::SharedRefBase::make<OsenseAidlHalReporter>();

    const std::string instance = std::string() + OsenseAidlHalReporter::descriptor + "/default";
    binder_status_t status =
            AServiceManager_addService(reporter->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
