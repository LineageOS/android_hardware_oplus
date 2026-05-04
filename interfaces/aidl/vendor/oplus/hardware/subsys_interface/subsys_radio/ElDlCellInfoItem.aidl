/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable ElDlCellInfoItem {
    byte techType;
    int bandNumber = 0;
    int arfcn = 0;
    int bandWidth = 0;
    byte isPrimary;
    byte isEndcAnchor;
    byte modulationType;
    byte transmissionMode;
    byte numberLayers;
    long cellId = 0;
    int pci = 0;
    int tac = 0;
    int lac = 0;
    float rsrp = 0.0f;
    float rsrq = 0.0f;
    float rssi = 0.0f;
    float rscp = 0.0f;
    float sinr = 0.0f;
}
