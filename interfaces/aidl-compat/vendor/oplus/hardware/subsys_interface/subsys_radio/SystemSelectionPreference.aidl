/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable SystemSelectionPreference {
    char modePref;
    long lteBandPref_1_64 = 0;
    long lteBandPref_65_128 = 0;
    long lteBandPref_129_192 = 0;
    long lteBandPref_193_256 = 0;
    long bandPref = 0;
    int acqOrderLen = 0;
    int[] acqOrder;
}
