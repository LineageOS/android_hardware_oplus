/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.communicationcenter;

@VintfStability
parcelable DmtpMessage {
    byte msgType;
    boolean local;
    int msgId;
    int reply;
    int reserve;
    byte[] data;
}
