/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable NvBackupStatisticsType {
    int totalCnt = 0;
    int backupCnt = 0;
    int[] unbackupItems;
    byte checkResult;
    byte year;
    byte month;
    byte day;
    String nvImageVersion;
}
