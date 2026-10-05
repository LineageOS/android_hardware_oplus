/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.touch;

@VintfStability
parcelable OplusTouchInfo {
    long time;
    int deviceId;
    int nodeFlag;
    int data;
    String info;
}
