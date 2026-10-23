/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable NasDeviceConfig {
    byte scm = 0;
    byte sci = 0;
    byte regHomeSid = 0;
    byte regForeignSid = 0;
    byte regForeignNid = 0;
    byte forceHdrscpConfigAt = 0;
}
