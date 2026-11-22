/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include <string_view>

// Mirrors provisioning_wire::AttestationIdInfo
struct DeviceInfo {
    std::string brand;
    std::string device;
    std::string product;
    std::string serial;
    std::string imei;
    std::string imei2;
    std::string meid;
    std::string meid2;
    std::string manufacturer;
    std::string model;

    std::string* field(std::string_view name);

    std::string format() const;

    // Compares the fields KmInstallKeybox compares (meid2 is not)
    bool matches(const DeviceInfo& other) const;
};
