/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable DataStallParam {
    byte td;
    byte type;
    boolean status = false;
    int expectedPacketCount = 0;
    int expectedDataRate = 0;
    int lostPacketCount = 0;
    int measureInterval = 0;
}
