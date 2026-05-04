/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable ImsMessage {
    char messageId;
    byte direction;
    byte sdpPresense;
    char responseCode;
    byte[] sipCallId;
    byte[] sipMessage;
}
