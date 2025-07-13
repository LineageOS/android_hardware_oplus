/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

@VintfStability
parcelable DataStallParam {
    byte td;
    byte type;
    boolean status;
    int expectedPacketCount;
    int expectedDataRate;
    int lostPacketCount;
    int measureInterval;
}
