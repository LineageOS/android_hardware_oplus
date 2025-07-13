/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

@VintfStability
parcelable OPLUS_rffe_data_type {
    byte ext;
    byte readwrite;
    byte channel;
    byte slave;
    char address;
    byte data;
    byte halfspeed;
}
