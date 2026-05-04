/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable CellSignalInfo {
    int rsrp = 0;
    int rsrq = 0;
    int sinr = 0;
    int snr = 0;
    int rssi = 0;
}
