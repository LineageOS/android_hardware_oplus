/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.cammidasservice;

@VintfStability
parcelable ExplorerModeInfo {
    int HDRRunTimes;
    int AINRRunTimes;
    int HDR_AINRRunTimes;
    int OtherModeRunTimes;
    double HDRDuration;
    double AINRDuration;
    double HDR_AINRDuration;
    double OtherModeDuration;
}
