/*
 * Copyright (C) 2024-2025 The LineageOS Project
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

#include <aidl/android/hardware/drm/IDrmFactory.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android/binder_manager.h>
#include <android/hardware/drm/1.0/IDrmFactory.h>

using android::sp;
using android::base::SetProperty;

const char APP_PACKAGE_NAME[] = "org.lineageos.wvmkiller";
const uint8_t WIDEVINE_UUID[] = {0xED, 0xEF, 0x8B, 0xA9, 0x79, 0xD6, 0x4A, 0xCE,
                                 0xA3, 0xC8, 0x27, 0xDC, 0xD5, 0x1D, 0x21, 0xED};

std::string systemIdAidl() {
    using aidl::android::hardware::drm::IDrmFactory;
    using aidl::android::hardware::drm::IDrmPlugin;
    using aidl::android::hardware::drm::Uuid;

    auto instance = std::format("{}/widevine", IDrmFactory::descriptor);

    if (!AServiceManager_isDeclared(instance.c_str())) {
        return "";
    }

    auto drmFactory =
            IDrmFactory::fromBinder(ndk::SpAIBinder(AServiceManager_getService(instance.c_str())));
    CHECK(drmFactory);

    Uuid uuid;
    uuid.uuid = std::to_array(WIDEVINE_UUID);

    std::shared_ptr<IDrmPlugin> drmPlugin;
    CHECK(drmFactory->createDrmPlugin(uuid, APP_PACKAGE_NAME, &drmPlugin).isOk());

    std::string systemId;
    CHECK(drmPlugin->getPropertyString("systemId", &systemId).isOk());

    return systemId;
}

std::string systemIdHidl() {
    using android::hardware::hidl_array;
    using android::hardware::hidl_string;
    using android::hardware::drm::V1_0::IDrmFactory;
    using android::hardware::drm::V1_0::IDrmPlugin;
    using android::hardware::drm::V1_0::Status;

    std::string ret;

    const auto callback = [&](Status status, const sp<IDrmPlugin>& drmPlugin) {
        CHECK(status == Status::OK);

        drmPlugin->getPropertyString("systemId", [&](Status status, const hidl_string& value) {
            CHECK(status == Status::OK);
            ret = value;
        });
    };

    const auto drmFactory = IDrmFactory::getService("widevine");

    if (drmFactory) {
        drmFactory->createPlugin(hidl_array<uint8_t, 16>(WIDEVINE_UUID), APP_PACKAGE_NAME,
                                 callback);
    }

    return ret;
}

int main() {
    if (systemIdAidl().size() > 5 || systemIdHidl().size() > 5) {
        SetProperty("vendor.wvm.disable_l1", "1");
    }

    return 0;
}
