/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.osense.client;

@VintfStability
parcelable OsenseAidlHalNotifyRequest {
    int msg_src;
    int msg_type;
    int param1;
    int param2;
    int param3;
    String param4;
}
