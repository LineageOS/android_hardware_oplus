/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.EccCatoryVal;
import vendor.oplus.hardware.subsys_interface.subsys_radio.EccMode;

@VintfStability
parcelable EccEntryInfo {
    char mcc;
    char mnc;
    byte mask = 0;
    String ecc;
    byte catLen = 0;
    byte special = 0;
    EccCatoryVal catVal;
    EccMode mode;
}
