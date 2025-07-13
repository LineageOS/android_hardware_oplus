/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

@VintfStability
parcelable OPLUS_RIL_Rx_Chain_info {
    byte is_radio_turned;
    int rx_pwr;
    int ecio;
    int rscp;
    int rsrp;
    int phase;
}
