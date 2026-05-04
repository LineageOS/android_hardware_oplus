/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@Backing(type="byte")
@VintfStability
enum EccMode {
    NOT_EMERGENCY = -1,
    EMERGENCY_GW = 0,
    EMERGENCY_1X = 1,
    EMERGENCY_GW_1X = 2,
}
