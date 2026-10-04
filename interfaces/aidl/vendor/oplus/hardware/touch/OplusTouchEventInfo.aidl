/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.touch;

@VintfStability
parcelable OplusTouchEventInfo {
    long time;
    int deviceId;
    int nodeFlag;
    int data;
    String info;
}
