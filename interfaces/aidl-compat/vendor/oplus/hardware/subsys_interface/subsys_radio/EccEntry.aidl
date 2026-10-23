/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable EccEntry {
    int mcc = 0;
    int mnc = 0;
    int mask = 0;
    String ecc;
    int catLen = 0;
    int catVal = 0;
    int mode = 0;
    int special = 0;
}
