/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.OPLUS_RIL_Ncell_Info;

@VintfStability
parcelable OPLUS_RIL_Radio_info {
    int rat;
    int mcc;
    int mnc;
    int lac;
    int cellid;
    int arfcn;
    int band;
    int rssi;
    int sinr;
    int rrstatus;
    int tx_power;
    int ncells_len;
    OPLUS_RIL_Ncell_Info[] ncells;
}
