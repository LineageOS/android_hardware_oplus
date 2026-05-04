/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable SatelliteCalDataStatusType {
    int label = 0;
    int backupSource = 0;
    String backupTimeMs;
    int backupSizeBytes = 0;
    int restoreSuccess = 0;
    int restoreBytes = 0;
    int restoreFailReason = 0;
}
