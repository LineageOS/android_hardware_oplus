/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

namespace oplus::power {

/**
 * Applies the stock charger cool down levels from charger_cool_down.conf
 * while the device is interactive and clears them otherwise. Does nothing
 * on devices without the config.
 */
void setChargerCoolDownActive(bool active);

}  // namespace oplus::power
