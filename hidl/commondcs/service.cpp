/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.oplus.hardware.commondcs@1.0-service"

#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>

#include "CommonDcsHalService.h"

using android::sp;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using vendor::oplus::hardware::commondcs::V1_0::ICommonDcsHalService;
using vendor::oplus::hardware::commondcs::V1_0::implementation::CommonDcsHalService;

int main() {
    sp<ICommonDcsHalService> commondcs = new CommonDcsHalService();
 
    configureRpcThreadpool(1, true /*callerWillJoin*/);
 
    const std::string instance = "commondcsservice";
    if (commondcs->registerAsService(instance) != android::OK) {
        LOG(ERROR) << "Can't register CommonDcsHalService HAL service";
        return 1;
    }
 
    joinRpcThreadpool();
 
    return 0;  // should never get here
}
 
