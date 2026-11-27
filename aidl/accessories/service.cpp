/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "OplusAccessories.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using aidl::vendor::lineage::accessories::OplusAccessories;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<OplusAccessories> accessories = ndk::SharedRefBase::make<OplusAccessories>();

    const std::string instance = std::string(OplusAccessories::descriptor) + "/default";
    binder_status_t status =
            AServiceManager_addService(accessories->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
