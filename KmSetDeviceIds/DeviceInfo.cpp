/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "DeviceInfo.h"

namespace {

struct Field {
    std::string_view name;
    std::string DeviceInfo::* member;
    bool sensitive;
};

constexpr Field kFields[] = {
        {"brand", &DeviceInfo::brand, false},
        {"device", &DeviceInfo::device, false},
        {"product", &DeviceInfo::product, false},
        {"serial", &DeviceInfo::serial, true},
        {"imei", &DeviceInfo::imei, true},
        {"imei2", &DeviceInfo::imei2, true},
        {"meid", &DeviceInfo::meid, true},
        {"meid2", &DeviceInfo::meid2, true},
        {"manufacturer", &DeviceInfo::manufacturer, false},
        {"model", &DeviceInfo::model, false},
};

}  // namespace

std::string* DeviceInfo::field(std::string_view name) {
    for (const auto& f : kFields) {
        if (f.name == name) return &(this->*f.member);
    }
    return nullptr;
}

std::string DeviceInfo::format() const {
    std::string out;
    for (const auto& f : kFields) {
        const std::string& value = this->*f.member;
        if (!out.empty()) out += "\n";
        out += "  ";
        out += f.name;
        out += ": ";
        if (f.sensitive) {
            out += value.empty() ? "<empty>" : "<redacted>";
        } else {
            out += value;
        }
    }
    return out;
}

bool DeviceInfo::matches(const DeviceInfo& other) const {
    return brand == other.brand && device == other.device && product == other.product &&
           serial == other.serial && imei == other.imei && imei2 == other.imei2 &&
           meid == other.meid && manufacturer == other.manufacturer && model == other.model;
}
