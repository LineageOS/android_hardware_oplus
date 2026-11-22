/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "DeviceInfo.h"

std::string formatDeviceInfo(const DeviceInfo& info) {
    std::string out;
    for (const auto& [key, value] : info) {
        bool sensitive = key == "serial" || key.rfind("imei", 0) == 0 || key.rfind("meid", 0) == 0;
        if (!out.empty()) out += "\n";
        out += "  " + key + ": ";
        if (sensitive) {
            out += value.empty() ? "<empty>" : "<redacted>";
        } else {
            out += value;
        }
    }
    return out;
}

bool matchDeviceInfo(const DeviceInfo& a, const DeviceInfo& b) {
    for (const char* key : {"brand", "device", "product", "serial", "manufacturer", "model", "imei",
                            "imei2", "meid"}) {
        auto ia = a.find(key), ib = b.find(key);
        std::string va = ia == a.end() ? "" : ia->second;
        std::string vb = ib == b.end() ? "" : ib->second;
        if (va != vb) return false;
    }
    return true;
}
