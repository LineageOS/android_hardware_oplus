/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.cammidasservice;

@VintfStability
parcelable ExplorerTempInfo {
    float[2] DDRTemp;
    float[2] NPUTemp;
    float[2] ISPTemp;
    float[2] MAXCPUTemp;
    float[2] surfaceTemp;
}
