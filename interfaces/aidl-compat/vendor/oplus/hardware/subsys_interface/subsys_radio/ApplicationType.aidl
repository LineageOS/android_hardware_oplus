/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@Backing(type="byte") @VintfStability
enum ApplicationType {
    CONV_AUDIO = 1,
    CONV_VIDEO = 2,
    FILE_TRANSFER = 7,
    STREAMING_AUDIO = 3,
    STREAMING_VIDEO = 4,
    TYPE_GAMING = 5,
    UNSPECIFIED = 0,
    WEB_BROWSING = 6,
}
