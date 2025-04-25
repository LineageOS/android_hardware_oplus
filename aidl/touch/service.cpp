/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.oplus"

#include <aidl/vendor/lineage/touch/IHighTouchPollingRate.h>
#include <aidl/vendor/lineage/touch/ITouchscreenGesture.h>
#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <android/binder_status.h>

#include "HighTouchPollingRate.h"
#include "TouchscreenGesture.h"

using aidl::vendor::lineage::touch::HighTouchPollingRate;
using aidl::vendor::lineage::touch::IHighTouchPollingRate;
using aidl::vendor::lineage::touch::ITouchscreenGesture;
using aidl::vendor::lineage::touch::TouchscreenGesture;

int main() {
    std::shared_ptr<IHighTouchPollingRate> highToushPollingRateService =
            ndk::SharedRefBase::make<HighTouchPollingRate>();

    std::shared_ptr<ITouchscreenGesture> touchscreenGestureService =
            ndk::SharedRefBase::make<TouchscreenGesture>();

    const std::string instance = std::string(IHighTouchPollingRate::descriptor) + "/default";
    binder_status_t status = AServiceManager_addService(
            highToushPollingRateService->asBinder().get(), instance.c_str());

    if (status != STATUS_OK) {
        LOG(WARNING) << "Can't register HighTouchPollingRate interface";
    }

    instance = std::string(ITouchscreenGesture::descriptor) + "/default";
    status = AServiceManager_addService(touchscreenGestureService->asBinder().get(),
                                        instance.c_str());

    if (status != STATUS_OK) {
        LOG(WARNING) << "Can't register TouchscreenGesture interface";
    }

    ABinderProcess_joinThreadPool();

    return 0;  // should never get here
}
