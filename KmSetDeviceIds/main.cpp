/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/logging.h>
#include <android-base/properties.h>

#include "KMHal.h"
#include "ModemIds.h"

using ::android::base::GetProperty;
using ::android::base::InitLogging;

int main(int argc, char** argv) {
    InitLogging(argv);

    bool force = argc == 3 && std::string(argv[2]) == "--force";
    if (argc != 2 && !force) {
        LOG(ERROR) << "Usage: " << argv[0] << " <serialno> [--force]";
        return 1;
    }

    ModemIds modem = fetchModemIds();

    DeviceInfo expected = {
            .brand = GetProperty("ro.product.brand", "ro.product.brand unavailable"),
            .device = GetProperty("ro.product.device", "ro.product.device unavailable"),
            .product = GetProperty("ro.product.name", "ro.product.name unavailable"),
            .serial = argv[1],
            .imei = modem.imei,
            .imei2 = modem.imei2,
            .meid = modem.meid,
            .manufacturer =
                    GetProperty("ro.product.manufacturer", "ro.product.manufacturer unavailable"),
            .model = GetProperty("ro.product.model", "ro.product.model unavailable"),
    };
    LOG(INFO) << "Device info:\n" << expected.format();

    auto km = KMHal::open();
    if (!km) return 1;

    auto result = km->getDeviceInfo();
    if (!result) return 1;

    if (auto* current = std::get_if<DeviceInfo>(&*result)) {
        if (current->matches(expected)) {
            LOG(INFO) << "Device IDs are already provisioned and match.";
            return 0;
        }
        LOG(WARNING) << "Device IDs are already provisioned and differ:\n" << current->format();
        if (!force) {
            return 0;
        }
        LOG(INFO) << "Reprovisioning (--force).";
    } else {
        LOG(INFO) << "Device info not provisioned (" << std::get<std::string>(*result)
                  << "), provisioning.";
    }

    if (int32_t ret = km->provisionDeviceIds(expected)) {
        LOG(ERROR) << "IKMHal_provisionDeviceIdsWithDualImei failed: " << ret;
        return 1;
    }
    LOG(INFO) << "Device info provisioning complete.";

    result = km->getDeviceInfo();
    if (!result) return 1;
    if (auto* err = std::get_if<std::string>(&*result)) {
        LOG(ERROR) << "Failed to read back device info: " << *err;
        return 1;
    }
    if (const auto& current = std::get<DeviceInfo>(*result); !current.matches(expected)) {
        LOG(ERROR) << "Provisioned device IDs differ:\n" << current.format();
        return 1;
    }
    LOG(INFO) << "Device IDs have been verified on the device.";
    return 0;
}
