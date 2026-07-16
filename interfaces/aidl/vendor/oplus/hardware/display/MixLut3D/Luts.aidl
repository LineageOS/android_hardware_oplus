/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.display.MixLut3D;

import vendor.oplus.hardware.display.MixLut3D.LutProperties;

@VintfStability
parcelable Luts {
    ParcelFileDescriptor fd;
    int[] offsets;
    LutProperties[] properties;
    int count;
}
