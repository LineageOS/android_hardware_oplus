/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

@VintfStability
parcelable EccEntryInfo {
    char mcc;
    char mnc;
    byte mask;
    String ecc;
    byte catLen;
    byte special;
    int catVal;
    byte mode;
}
