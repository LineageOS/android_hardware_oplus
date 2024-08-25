/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.osense.client;

@VintfStability
parcelable OsenseAidlHalSaRequest {
    String scene;
    String action;
    int timeout;
    long request;
}
