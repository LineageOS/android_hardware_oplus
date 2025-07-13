/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

@VintfStability
@Backing(type="int")
enum Oplus_change_duration_type {
    OPLUS_CHANGE_DURATION_NONE = -1,
    OPLUS_CHANGE_DURATION_POWER_CYCLE = 0,
    OPLUS_CHANGE_DURATION_PERMANENT = 1,
    OPLUS_CHANGE_DURATION_MAX = 2
}
