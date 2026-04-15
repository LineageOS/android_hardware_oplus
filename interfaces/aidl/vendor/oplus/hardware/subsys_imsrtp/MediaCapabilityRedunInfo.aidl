/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_imsrtp;

@VintfStability
parcelable MediaCapabilityRedunInfo {
    int subId = 0;
    int reason = 0;
    boolean isMediaQualityCheckEnabled = false;
    boolean isValidRsrpThreshold = false;
    int lteStartTh = 0;
    int lteStopTh = 0;
    int nrStartTh = 0;
    int nrStopTh = 0;
}
