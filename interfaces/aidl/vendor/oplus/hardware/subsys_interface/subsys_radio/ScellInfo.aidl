/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable ScellInfo {
    char scc_phy_cellid;
    int scc_ul_channel = 0;
    int scc_dl_channel = 0;
    int rsrp = 0;
    int rsrq = 0;
    byte rs_sinr;
}
