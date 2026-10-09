/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>

// Builds thermal-engine.conf contents from stock oplus thermal policies, returns an empty string on
// failure. projectName selects the horae_target.conf entry, the first one with shell sensors is
// used if it's empty or missing.
std::string GenerateThermalEngineConfig(const char* controlConfigPath, const char* horaeConfigPath,
                                        const std::string& projectName);
