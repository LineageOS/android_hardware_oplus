/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable QrxlvminRspInfo {
    int rat = 0;
    int lteQrxlevGood = 0;
    int lteQrxlevBad = 0;
    int lteQrxlevVal = 0;
    int nrQrxlevVal = 0;
    int nrQrxlevGood = 0;
    int nrQrxlevBad = 0;
    int nrQqualVal = 0;
    int nrQqualGood = 0;
    int nrQqualBad = 0;
}
