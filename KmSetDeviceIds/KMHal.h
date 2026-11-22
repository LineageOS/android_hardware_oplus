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

    enum class QueryResult {
        kProvisioned,     // info is filled
        kNotProvisioned,  // err is filled
        kUnsupported,     // the TA doesn't support querying device info (e.g. sm8750)
        kError,
    };

    // Queries provisioned device info
    QueryResult getDeviceInfo(DeviceInfo& info, std::string& err);

    // Provisions device IDs, returns 0 on success or the TA error code
    int32_t provisionDeviceIds(const DeviceInfo& info);

  private:
    explicit KMHal(ObjectRef km) : km_(std::move(km)) {}

    // Sends a command, returns 0 and fills rsp with the response payload, the TA status if
    // it rejected the command, or the invoke error
    int32_t sendCmd(const std::vector<uint8_t>& req, std::vector<uint8_t>& rsp);

    ObjectRef km_;
};
