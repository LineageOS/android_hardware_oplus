/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable RfTxInfo {
    int radioTech = 0;
    int band = 0;
    int bandwidth = 0;
    int ulChannel = 0;
    int dlChannel = 0;
    int dbm = 0;
    int onOff = 0;
    int ant = 0;
    int startRb = 0;
    int rbNumber = 0;
    int subband = 0;
    int paIndex = 0;
    int dutycycle = 0;
    int waveform = 0;
    int modulation = 0;
    int scs = 0;
}
