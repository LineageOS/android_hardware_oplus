/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.CellSignalInfo;

@VintfStability
parcelable ServingCellInfo {
    int enter_time = 0;
    int rat = 0;
    int mcc = 0;
    int mnc = 0;
    int tac = 0;
    long cid = 0;
    int pci = 0;
    int band = 0;
    int arfcn = 0;
    int bandwidth = 0;
    CellSignalInfo signal_info;
    int ul_arfcn = 0;
    int ul_bandwidth = 0;
    int doppler = 0;
    int nr5g_scs = 0;
    byte is_uli_cell;
    byte is_hst_cell;
    byte is_3digit_mnc;
}
