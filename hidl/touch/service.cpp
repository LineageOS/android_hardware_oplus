/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "vendor.lineage.touch@1.0-service.oplus"

#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>
#include "HighTouchPollingRate.h"
#include "TouchscreenGesture.h"

using android::sp;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using vendor::lineage::touch::V1_0::IHighTouchPollingRate;
using vendor::lineage::touch::V1_0::ITouchscreenGesture;
using vendor::lineage::touch::V1_0::implementation::HighTouchPollingRate;
using vendor::lineage::touch::V1_0::implementation::TouchscreenGesture;

int main() {
    sp<IHighTouchPollingRate> highToushPollingRateService = new HighTouchPollingRate();
    sp<ITouchscreenGesture> gestureService = new TouchscreenGesture();

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    if (highToushPollingRateService->registerAsService() != android::OK) {
        LOG(WARNING) << "Can't register HighTouchPollingRate HAL service";
    }

    if (gestureService->registerAsService() != android::OK) {
        LOG(ERROR) << "Can't register TouchscreenGesture HAL service";
        return 1;
    }

    joinRpcThreadpool();

    return 0;  // should never get here
}
