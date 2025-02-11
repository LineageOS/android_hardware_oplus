/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.oplus_fusionlight;

import vendor.lineage.oplus_fusionlight.ScreenSample;

@VintfStability
oneway interface IScreenSampleCallback {
    void onSample(in ScreenSample sample);
}
