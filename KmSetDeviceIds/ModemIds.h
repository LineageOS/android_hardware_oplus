/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>

struct ModemIds {
    std::string imei, imei2, meid;
};

ModemIds fetchModemIds();
