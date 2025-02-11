/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.oplus_fusionlight;

import vendor.lineage.oplus_fusionlight.IScreenSampleCallback;
import vendor.lineage.oplus_fusionlight.SamplingConfig;

@VintfStability
interface IScreenSampler {
    /** Starts one sampling session. The returned ID is positive and unique while active. */
    long startSampling(in SamplingConfig config, IScreenSampleCallback callback);

    /** Stops the session and prevents any new callback from being queued. */
    void stopSampling(long sessionId);
}
