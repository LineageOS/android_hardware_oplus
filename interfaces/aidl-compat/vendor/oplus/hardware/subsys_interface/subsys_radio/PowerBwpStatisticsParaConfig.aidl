/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable PowerBwpStatisticsParaConfig {
    int ts = 0;
    int optMaxccDuration = 0;
    int maxccConfigDuration = 0;
    int maxccConfigCount = 0;
    int optMaxMimoConfigDuration = 0;
    int maxmimoConfigCount = 0;
    int maxmimoConfigDuration = 0;
    int optMaxbwDuration = 0;
    int maxbwConfigDuration = 0;
    int maxbwConfigCount = 0;
}
