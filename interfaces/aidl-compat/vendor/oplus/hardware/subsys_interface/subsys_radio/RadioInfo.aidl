/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.NcellInfo;

@VintfStability
parcelable RadioInfo {
    int rat = 0;
    int mcc = 0;
    int mnc = 0;
    int lac = 0;
    int cellid = 0;
    int arfcn = 0;
    int band = 0;
    int rssi = 0;
    int sinr = 0;
    int rrstatus = 0;
    int tx_power = 0;
    int ncells_len = 0;
    NcellInfo[] ncells;
}
