/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.QmsPlmn;

@VintfStability
parcelable NfList {
    int arfcn;
    int band;
    byte scs;
    int plmnSize;
    QmsPlmn[] plmn;
}
