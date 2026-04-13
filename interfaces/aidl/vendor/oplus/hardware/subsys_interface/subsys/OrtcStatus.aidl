/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys;

@VintfStability
parcelable OrtcStatus {
    int subId = 0;
    int callId = 0;
    int mediaId = 0;
    int newConnectStatus = 0;
    int serviceStatus = 0;
}
