/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.touch;

@VintfStability
parcelable OplusTouchInfo {
    String info;
    long time;
    int deviceId;
    int nodeFlag;
    int data;
}
