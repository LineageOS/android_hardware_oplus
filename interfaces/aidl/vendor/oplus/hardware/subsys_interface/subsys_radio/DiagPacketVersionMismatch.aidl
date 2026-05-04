/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable DiagPacketVersionMismatch {
    byte pktType;
    int pktId = 0;
    int oldVersion = 0;
    int newVersion = 0;
}
