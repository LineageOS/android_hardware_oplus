/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdint.h>

#include <optional>
#include <string>
#include <vector>

#include "DeviceInfo.h"
#include "Mink.h"

class KMHal {
  public:
    static std::optional<KMHal> open();

    // Queries provisioned device info. Returns true and fills info if provisioned, false and
    // fills err if not provisioned or the response couldn't be decoded, nullopt if TA
    // communication failed.
    std::optional<bool> getDeviceInfo(DeviceInfo& info, std::string& err);

    int32_t provisionDeviceIds(const DeviceInfo& info);

  private:
    explicit KMHal(ObjectRef km) : km_(std::move(km)) {}

    std::optional<std::vector<uint8_t>> sendCmd(const std::vector<uint8_t>& req);

    ObjectRef km_;
};
