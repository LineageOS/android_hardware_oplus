/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable LockCellInfo {
    int rat = 0;
    int arfcn = 0;
    int pci = 0;
    int scs = 0;
    int band = 0;
    int enableGsm1900 = 0;
    int lockMode = 0;
    int enforce = 0;
}
