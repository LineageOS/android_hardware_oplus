/*
 * Copyright (C) 2024-2026 The LineageOS Project
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

#include <thread>

using android::sp;
using android::base::SetProperty;
using std::chrono_literals::operator""s;

const char APP_PACKAGE_NAME[] = "org.lineageos.wvmkiller";
const uint8_t WIDEVINE_UUID[] = {0xED, 0xEF, 0x8B, 0xA9, 0x79, 0xD6, 0x4A, 0xCE,
                                 0xA3, 0xC8, 0x27, 0xDC, 0xD5, 0x1D, 0x21, 0xED};

// How long to wait for rkpd to provision a provisioning 4.0 device (5 minutes).
const auto PROV40_POLL_INTERVAL = 10s;
const int PROV40_POLL_ATTEMPTS = 30;

std::string propertyAidl(const std::string& name) {
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

    std::string value;
    if (!drmPlugin->getPropertyString(name, &value).isOk()) {
        return "";
    }

    return value;
}

std::string systemIdHidl() {
    using android::hardware::hidl_array;
    using android::hardware::hidl_string;
    using android::hardware::drm::V1_0::IDrmFactory;
    using android::hardware::drm::V1_0::IDrmPlugin;
    using android::hardware::drm::V1_0::Status;

    std::string systemId;

    const auto callback = [&](Status status, const sp<IDrmPlugin>& drmPlugin) {
        CHECK(status == Status::OK);

        drmPlugin->getPropertyString("systemId", [&](Status status, const hidl_string& value) {
            CHECK(status == Status::OK);
            systemId = value;
        });
    };

    const auto drmFactory = IDrmFactory::getService("widevine");

    if (drmFactory) {
        drmFactory->createPlugin(hidl_array<uint8_t, 16>(WIDEVINE_UUID), APP_PACKAGE_NAME,
                                 callback);
    }

    return systemId;
}

bool isUnprovisioned() {
    return propertyAidl("systemId").size() > 5 || systemIdHidl().size() > 5;
}

int main() {
    if (!isUnprovisioned()) {
        return 0;
    }

    // Provisioning 4.0 devices report a placeholder system ID until rkpd fetches their
    // certificate after boot, which it only does while L1 is enabled.
    if (propertyAidl("provisioningModel") == "BootCertificateChain") {
        for (int i = 0; i < PROV40_POLL_ATTEMPTS; i++) {
            std::this_thread::sleep_for(PROV40_POLL_INTERVAL);
            if (!isUnprovisioned()) {
                LOG(INFO) << "Widevine L1 got provisioned, keeping it enabled";
                return 0;
            }
        }
        LOG(INFO) << "Widevine L1 is still not provisioned, disabling it";
    }

    SetProperty("vendor.wvm.disable_l1", "1");

    return 0;
}
