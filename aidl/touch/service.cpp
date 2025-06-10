/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.oplus"

#include "GloveMode.h"
#include "HighTouchPollingRate.h"
#include "TouchscreenGesture.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using aidl::vendor::lineage::touch::GloveMode;
using aidl::vendor::lineage::touch::HighTouchPollingRate;
using aidl::vendor::lineage::touch::TouchscreenGesture;
using aidl::vendor::oplus::hardware::touch::IOplusTouch;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    const std::string instance = std::string() + IOplusTouch::descriptor + "/default";
    std::shared_ptr<IOplusTouch> oplusTouch =
            USE_OPLUSTOUCH ? IOplusTouch::fromBinder(ndk::SpAIBinder(
                                     AServiceManager_waitForService(instance.c_str())))
                           : nullptr;

    std::shared_ptr<GloveMode> gm =
            ENABLE_GM ? ndk::SharedRefBase::make<GloveMode>(oplusTouch) : nullptr;
    std::shared_ptr<HighTouchPollingRate> htpr =
            ENABLE_HTPR ? ndk::SharedRefBase::make<HighTouchPollingRate>(oplusTouch) : nullptr;
    std::shared_ptr<TouchscreenGesture> tg =
            ENABLE_TG ? ndk::SharedRefBase::make<TouchscreenGesture>(oplusTouch) : nullptr;

    if (gm) {
        const std::string instance = std::string(GloveMode::descriptor) + "/default";
        const binder_status_t status =
                AServiceManager_addService(gm->asBinder().get(), instance.c_str());
        CHECK_EQ(status, STATUS_OK) << "Failed to add service " << instance << " " << status;
    }

    if (htpr) {
        const std::string instance = std::string(HighTouchPollingRate::descriptor) + "/default";
        const binder_status_t status =
                AServiceManager_addService(htpr->asBinder().get(), instance.c_str());
        CHECK_EQ(status, STATUS_OK) << "Failed to add service " << instance << " " << status;
    }

    if (tg) {
        const std::string instance = std::string(TouchscreenGesture::descriptor) + "/default";
        const binder_status_t status =
                AServiceManager_addService(tg->asBinder().get(), instance.c_str());
        CHECK_EQ(status, STATUS_OK) << "Failed to add service " << instance << " " << status;
    }

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
