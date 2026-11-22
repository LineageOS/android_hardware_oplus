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

// Client for the IKMHal object of the QTI KeyMint TA
class KMHal {
  public:
    // Opens the IKMHal object of the keymaster64 TA
    static std::optional<KMHal> open();

    // Queries provisioned device info. Returns true and fills info if provisioned, false and
    // fills err if not provisioned or the response couldn't be decoded, nullopt if TA
    // communication failed.
    std::optional<bool> getDeviceInfo(DeviceInfo& info, std::string& err);

    // Provisions device IDs, returns 0 on success or the TA error code
    int32_t provisionDeviceIds(const DeviceInfo& info);

  private:
    explicit KMHal(ObjectRef km) : km_(std::move(km)) {}

    // Sends a command, returns the response payload
    std::optional<std::vector<uint8_t>> sendCmd(const std::vector<uint8_t>& req);

    ObjectRef km_;
};
