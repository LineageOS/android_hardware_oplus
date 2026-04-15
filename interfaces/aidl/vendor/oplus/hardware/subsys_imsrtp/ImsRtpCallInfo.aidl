/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_imsrtp;

@VintfStability
parcelable ImsRtpCallInfo {
    int subId = 0;
    int callId = 0;
    int mediaId = 0;
    int callState = 0;
    String ortcUserId;
    String ortcUserName;
    String privateKey;
    String appId;
    String serverUrl;
    String roomId;
    int allCallEnd = 0;
    int linkTypeValid = 0;
    int linkType = 0;
}
