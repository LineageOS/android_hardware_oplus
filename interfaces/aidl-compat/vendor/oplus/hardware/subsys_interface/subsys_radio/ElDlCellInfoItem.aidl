/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable ElDlCellInfoItem {
    byte techType = 0;
    int bandNumber = 0;
    int arfcn = 0;
    int bandWidth = 0;
    byte isPrimary = 0;
    byte isEndcAnchor = 0;
    byte modulationType = 0;
    byte transmissionMode = 0;
    byte numberLayers = 0;
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
