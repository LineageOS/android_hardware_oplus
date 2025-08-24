/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.performance;

@VintfStability
parcelable ThreadInfo {
    int pid = 0;
    int tid = 0;
    int SourcePid = 0;
    int utils = 0;
    int nice = 0;
    int WakeCnt = 0;
    int totalWakeCnt = 0;
}
