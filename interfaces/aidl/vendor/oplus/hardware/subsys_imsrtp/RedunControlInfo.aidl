/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_imsrtp;

@VintfStability
parcelable RedunControlInfo {
    int subId = 0;
    int callId = 0;
    int mediaId = 0;
    int controlId = 0;
    int reason = 0;
}
