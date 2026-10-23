/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable NrCellInfo {
    int band = 0;
    int bandwidth = 0;
    int arfcn = 0;
    int pci = 0;
    int rsrp = 0;
    int rsrq = 0;
    int sinr = 0;
    int bler = 0;
    int scs = 0;
    int beam = 0;
}
