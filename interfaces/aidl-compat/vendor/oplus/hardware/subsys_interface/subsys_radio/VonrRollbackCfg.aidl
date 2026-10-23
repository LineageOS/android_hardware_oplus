/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable VonrRollbackCfg {
    int featureSwitch = 0;
    int backoffNrRsrpTh = 0;
    int backoffNrSinrTh = 0;
    int backoffLteRsrpTh = 0;
    int backoffLteRsrqTh = 0;
    int backoffLteSinrTh = 0;
    int backoffDurTh = 0;
    int rollbackNrRsrpTh = 0;
    int rollbackNrSinrTh = 0;
    int rollbackDurTh = 0;
    int maxNum = 0;
}
