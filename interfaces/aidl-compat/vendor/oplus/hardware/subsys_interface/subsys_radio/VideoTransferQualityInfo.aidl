/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable VideoTransferQualityInfo {
    int totalScore = 0;
    int movingAvgX100 = 0;
    int slopeX100 = 0;
    int dir = 0;
}
