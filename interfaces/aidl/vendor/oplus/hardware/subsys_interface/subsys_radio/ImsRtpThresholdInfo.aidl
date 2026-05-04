/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable ImsRtpThresholdInfo {
    int reason = 0;
    int lteStartTh = 0;
    int lteStopTh = 0;
    int nrStartTh = 0;
    int nrStopTh = 0;
    int lteStartTime = 0;
    int lteStopTime = 0;
    int nrStartTime = 0;
    int nrStopTime = 0;
}
