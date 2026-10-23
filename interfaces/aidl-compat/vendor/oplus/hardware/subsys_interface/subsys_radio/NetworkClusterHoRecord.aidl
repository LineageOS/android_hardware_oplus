/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable NetworkClusterHoRecord {
    byte label = 0;
    byte type = 0;
    float[] embedding;
    int srcArfcn = 0;
    char srcPci;
    int targetArfcn = 0;
    char targetPci;
    int tac = 0;
    long cid = 0;
    int gpsFenceId = 0;
    int wifiFenceId = 0;
}
