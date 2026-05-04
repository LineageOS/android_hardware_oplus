/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable TxAdcInfo {
    int radioTech;
    int band = 0;
    int bandwidth = 0;
    int ulChannel = 0;
    int dlChannel = 0;
    int dbm = 0;
    int ant = 0;
    int subband = 0;
    int paIndex = 0;
}
