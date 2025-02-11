/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.oplus_fusionlight;

@VintfStability
parcelable SamplingConfig {
    /** Half-open crop rectangle in the profile's reference display coordinate space. */
    int left;
    int top;
    int right;
    int bottom;

    /** Reference display size used to map the crop to the active display. */
    int referenceWidth;
    int referenceHeight;

    /** Requested interval between samples, in milliseconds. */
    int samplingPeriodMillis;
}
