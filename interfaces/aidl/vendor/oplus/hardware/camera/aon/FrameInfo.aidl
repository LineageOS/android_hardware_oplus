/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.camera.aon;

@VintfStability
parcelable FrameInfo {
    int frameWidth;
    int frameHeight;
    int[] frameRates;
}
