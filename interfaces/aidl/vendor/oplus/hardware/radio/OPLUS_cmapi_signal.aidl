/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

@VintfStability
parcelable OPLUS_cmapi_signal {
    byte is_in_traffic;
    int tx_pwr;
    int[] rsrp;
}
