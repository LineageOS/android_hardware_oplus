/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.Oplus_5g_band_mask_type;

@VintfStability
parcelable Oplus_5g_band_type {
    boolean valid;
    Oplus_5g_band_mask_type band_mask;
}
