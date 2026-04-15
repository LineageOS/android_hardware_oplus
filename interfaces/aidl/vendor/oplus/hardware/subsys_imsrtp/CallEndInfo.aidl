/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_imsrtp;

@VintfStability
parcelable CallEndInfo {
    int subId = 0;
    int callId = 0;
    int mediaId = 0;
    int source = 0;
    int type = 0;
    String result;
}
