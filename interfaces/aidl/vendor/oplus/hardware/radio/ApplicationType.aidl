/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

@VintfStability
@Backing(type="byte")
enum ApplicationType {
    UNSPECIFIED = 0,
    CONV_AUDIO = 1,
    CONV_VIDEO = 2,
    STREAMING_AUDIO = 3,
    STREAMING_VIDEO = 4,
    TYPE_GAMING = 5,
    WEB_BROWSING = 6,
    FILE_TRANSFER = 7
}
