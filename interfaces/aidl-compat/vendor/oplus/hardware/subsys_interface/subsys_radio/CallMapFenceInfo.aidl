/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable CallMapFenceInfo {
    int gpsFenceID = 0;
    int wifiFenceID = 0;
    int wifi3FenceID = 0;
    int pci = 0;
    int arfcn = 0;
    int tac = 0;
    long cid = 0;
    int issueType = 0;
    int targetPci1 = 0;
    int targetArfcn1 = 0;
    int targetPci2 = 0;
    int targetArfcn2 = 0;
    int targetPci3 = 0;
    int targetArfcn3 = 0;
    int rlfCause1 = 0;
    int rlfCause2 = 0;
    int rlfCause3 = 0;
    int abnormalEvent1 = 0;
    int abnormalEvent2 = 0;
    int abnormalEvent3 = 0;
    int gpsConfidence = 0;
    int wifiConfidence = 0;
    int wifi3Confidence = 0;
    int fenceStatus = 0;
}
