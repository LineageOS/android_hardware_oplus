/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.oplus_fusionlight;

@VintfStability
parcelable ScreenSample {
    /** Sampling session that produced this result. */
    long sessionId;

    /** CLOCK_BOOTTIME bounds of the display capture. */
    long frameStartNs;
    long frameEndNs;

    /** Mean crop-channel values in the unmodified 0..255 screenshot domain. */
    float r;
    float g;
    float b;
}
