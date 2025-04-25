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
    std::shared_ptr<HighTouchPollingRate> highTouchPollingRateService =
            ndk::SharedRefBase::make<HighTouchPollingRate>();

    std::shared_ptr<TouchscreenGesture> touchscreenGestureService =
            ndk::SharedRefBase::make<TouchscreenGesture>();

    const std::string instance = std::string(HighTouchPollingRate::descriptor) + "/default";
    binder_status_t status = AServiceManager_addService(
            highTouchPollingRateService->asBinder().get(), instance.c_str());

    if (status != STATUS_OK) {
        LOG(WARNING) << "Can't register IHighTouchPollingRate/default";
    }

    const std::string instance2 = std::string(TouchscreenGesture::descriptor) + "/default";
    binder_status_t status2 = AServiceManager_addService(
            touchscreenGestureService->asBinder().get(), instance2.c_str());

    if (status2 != STATUS_OK) {
        LOG(WARNING) << "Can't register ITouchscreenGesture/default";
    }

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
