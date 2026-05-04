/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable PowerSmartIdleStatisticsParaConfig {
    int netIanctiveTimer = 0;
    int permanentInactiveTimer = 0;
    int temporaryInactiveTimer = 0;
    int permanentRrcSaveCnt = 0;
    int temporaryRrcSaveCnt = 0;
    int permanentRrcSaveDur = 0;
    int temporaryRrcSaveDur = 0;
}
