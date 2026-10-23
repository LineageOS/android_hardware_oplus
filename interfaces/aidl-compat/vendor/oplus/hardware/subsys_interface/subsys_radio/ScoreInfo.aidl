/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable ScoreInfo {
    int policyId = 0;
    int arfcn = 0;
    int pci = 0;
    int uid = 0;
    int cause = 0;
    int technology = 0;
    int scoreLevel = 0;
    int lastScoreValue = 0;
}
