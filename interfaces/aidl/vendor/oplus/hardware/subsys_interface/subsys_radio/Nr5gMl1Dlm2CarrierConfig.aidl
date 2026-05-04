/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable Nr5gMl1Dlm2CarrierConfig {
    byte ccId;
    byte sccId;
    int pci = 0;
    int dlState = 0;
    int dlEarfcn = 0;
    int band = 0;
    int dlBandwidth = 0;
    int ulState = 0;
    int ulBandwith = 0;
}
