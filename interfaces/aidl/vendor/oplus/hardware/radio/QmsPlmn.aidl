/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

@VintfStability
parcelable QmsPlmn {
    int mcc;
    int mnc;
    boolean pcsDigit;
}
