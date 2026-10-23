/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable ImsRtpState {
    int totalLost = 0;
    int totalPacketsCount = 0;
    int averageRelJitter = 0;
    int averageFrameDelay = 0;
}
