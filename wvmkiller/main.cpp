/*
 * Copyright (C) 2024 The LineageOS Project
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

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android/hardware/drm/1.0/IDrmFactory.h>

using android::sp;
using android::base::SetProperty;
using android::hardware::hidl_array;
using android::hardware::hidl_string;
using android::hardware::drm::V1_0::IDrmFactory;
using android::hardware::drm::V1_0::IDrmPlugin;
using android::hardware::drm::V1_0::Status;

int main() {
    const uint8_t WIDEVINE_UUID[] = {0xed, 0xef, 0x8b, 0xa9, 0x79, 0xd6, 0x4a, 0xce,
                                     0xa3, 0xc8, 0x27, 0xdc, 0xd5, 0x1d, 0x21, 0xed};

    const auto callback = [](Status status, const sp<IDrmPlugin>& drmPlugin) {
        CHECK(status == Status::OK);

        drmPlugin->getPropertyString("systemId", [](Status status, const hidl_string& value) {
            CHECK(status == Status::OK);

            if (value.size() > 5) {
                SetProperty("vendor.wvm.disable_l1", "1");
            }
        });
    };

    const auto drmFactory = IDrmFactory::getService("widevine");
    CHECK(drmFactory.get() != nullptr);

    drmFactory->createPlugin(hidl_array<uint8_t, 16>(WIDEVINE_UUID), "org.lineageos.wvmkiller",
                             callback);

    return 0;
}
