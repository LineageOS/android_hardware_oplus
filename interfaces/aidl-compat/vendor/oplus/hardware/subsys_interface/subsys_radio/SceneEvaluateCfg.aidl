/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable SceneEvaluateCfg {
    int detectWindow = 0;
    int powerLackThreshold = 0;
    int powerFallbackThreshold = 0;
    int powerEvaluateTime = 0;
    int monitorCount = 0;
    int monitorSuddenCount = 0;
}
