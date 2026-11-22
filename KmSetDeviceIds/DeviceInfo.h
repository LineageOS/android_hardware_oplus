/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <map>
#include <string>

// Attestation device IDs, keyed by provisioning_wire::AttestationIdInfo field names
using DeviceInfo = std::map<std::string, std::string>;

// Formats device info for logging, with unique identifiers redacted
std::string formatDeviceInfo(const DeviceInfo& info);

// Compares the fields KmInstallKeybox compares (meid2 is not)
bool matchDeviceInfo(const DeviceInfo& a, const DeviceInfo& b);
