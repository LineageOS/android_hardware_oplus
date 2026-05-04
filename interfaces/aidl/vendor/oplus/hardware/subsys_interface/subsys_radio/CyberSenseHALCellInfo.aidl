/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.CyberSenseHalBeamInfo;

@VintfStability
parcelable CyberSenseHALCellInfo {
    int rat = 0;
    long timeStamp = 0;
    long cid = 0;
    int pci = 0;
    int mcc = 0;
    int mnc = 0;
    int tac = 0;
    int arfcn = 0;
    int bandWidth = 0;
    int rsrp = 0;
    int rsrq = 0;
    int rssi = 0;
    int ssbIndex = 0;
    int isCA = 0;
    int isNSA = 0;
    int dopplerOffset = 0;
    @nullable CyberSenseHalBeamInfo[] beams;
}
