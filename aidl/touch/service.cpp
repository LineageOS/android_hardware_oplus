/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.oplus"

#include "HighTouchPollingRate.h"
#include "TouchscreenGesture.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using aidl::vendor::lineage::touch::HighTouchPollingRate;
using aidl::vendor::lineage::touch::TouchscreenGesture;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    std::shared_ptr<HighTouchPollingRate> htpr =
            ENABLE_HTPR ? ndk::SharedRefBase::make<HighTouchPollingRate>() : nullptr;
    std::shared_ptr<TouchscreenGesture> tg =
            ENABLE_TG ? ndk::SharedRefBase::make<TouchscreenGesture>() : nullptr;

    if (htpr) {
        const std::string instance = std::string(HighTouchPollingRate::descriptor) + "/default";
        const binder_status_t status =
                AServiceManager_addService(htpr->asBinder().get(), instance.c_str());
        if (status != STATUS_OK) {
            LOG(WARNING) << "Can't register IHighTouchPollingRate/default";
        }
    }

    if (tg) {
        const std::string instance = std::string(TouchscreenGesture::descriptor) + "/default";
        const binder_status_t status =
                AServiceManager_addService(tg->asBinder().get(), instance.c_str());
        if (status != STATUS_OK) {
            LOG(WARNING) << "Can't register ITouchscreenGesture/default";
        }
    }

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
