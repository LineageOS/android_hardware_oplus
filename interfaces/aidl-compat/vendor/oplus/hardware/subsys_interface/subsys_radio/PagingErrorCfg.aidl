/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable PagingErrorCfg {
    int featureSwitch = 0;
    int pagingErrorCnt = 0;
    int pagingErrorRate = 0;
    int scellRSRP = 0;
    int ncellRSRP = 0;
    int barCellTime = 0;
}
