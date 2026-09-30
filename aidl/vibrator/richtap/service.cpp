/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include <string>
#include <thread>

#include "RichtapVibrator.h"
#include "Vibrator.h"

using aidl::android::hardware::vibrator::Vibrator;
using aidl::vendor::aac::hardware::richtap::vibrator::RichtapVibrator;

// Set by init once haptics calibration has completed.
constexpr char kHapticCalibrateProp[] = "vendor.haptic.calibrate.done";

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    std::shared_ptr<Vibrator> vib = ndk::SharedRefBase::make<Vibrator>();
    ndk::SpAIBinder vibBinder = vib->asBinder();

    // Attach the RichTap extension to the standard vibrator service; the framework reaches it
    // through IBinder.getExtension().
    std::shared_ptr<RichtapVibrator> cvib = ndk::SharedRefBase::make<RichtapVibrator>();
    CHECK(STATUS_OK == AIBinder_setExtension(vibBinder.get(), cvib->asBinder().get()));

    const std::string instance = std::string() + Vibrator::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(vib->asBinder().get(), instance.c_str());
    CHECK(status == STATUS_OK);

    // Defer RichTap init until haptics calibration is done, with a bounded fallback in case the
    // property is never set.
    std::thread initThread([&]() {
        using namespace std::chrono_literals;
        ::android::base::WaitForProperty(kHapticCalibrateProp, "1", 60s);
        cvib->init(nullptr);
    });
    initThread.detach();

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
