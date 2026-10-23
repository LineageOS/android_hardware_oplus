/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.a2dp_offload;

@VintfStability
@Backing(type="int")
enum Lhdcv5QualityIndex {
    UNKNOWN = 0,
    QUALITY_LOW0 = 1,
    QUALITY_LOW1 = 2,
    QUALITY_LOW2 = 3,
    QUALITY_LOW3 = 4,
    QUALITY_LOW4 = 5,
    QUALITY_LOW = 6,
    QUALITY_MID = 7,
    QUALITY_HIGH = 8,
    QUALITY_HIGH1 = 9,
    QUALITY_HIGH2 = 10,
    QUALITY_HIGH3 = 11,
    QUALITY_HIGH4 = 12,
    QUALITY_HIGH5 = 13,
    QUALITY_ABR = 14,
    QUALITY_UNLIMIT = 15,
    QUALITY_CTRL_RESET_ABR = 128,
    QUALITY_CTRL_END = 129,
    QUALITY_INVALID = 130
}
