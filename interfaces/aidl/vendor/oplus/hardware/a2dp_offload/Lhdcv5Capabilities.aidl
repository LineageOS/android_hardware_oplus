/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.a2dp_offload;

import vendor.oplus.hardware.a2dp_offload.Lhdcv5QualityIndex;
import vendor.oplus.hardware.a2dp_offload.Lhdcv5Specific;

@VintfStability
parcelable Lhdcv5Capabilities {
    int[] sampleRateHz;
    byte[] channelMode;
    byte[] bitsPerSample;
    byte[] codecVersion;
    Lhdcv5QualityIndex[] qualityIndex;
    Lhdcv5QualityIndex[] maxQualityIndex;
    Lhdcv5QualityIndex[] minQualityIndex;
    byte[] frameDuration;
    byte[] dataInterval;
    Lhdcv5Specific[] codecSpecific_1;
    Lhdcv5Specific[] codecSpecific_2;
    byte[] metaData;
}
