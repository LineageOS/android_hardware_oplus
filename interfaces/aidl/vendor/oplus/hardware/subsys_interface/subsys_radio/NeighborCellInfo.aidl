/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.CellSignalInfo;

@VintfStability
parcelable NeighborCellInfo {
    int rat = 0;
    int arfcn = 0;
    int pci = 0;
    CellSignalInfo signal_info;
}
