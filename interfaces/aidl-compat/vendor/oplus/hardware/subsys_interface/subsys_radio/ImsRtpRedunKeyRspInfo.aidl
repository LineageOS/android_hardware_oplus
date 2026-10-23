/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable ImsRtpRedunKeyRspInfo {
    int sessionId = 0;
    int result = 0;
    int cause = 0;
    int keyType = 0;
}
