/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable VonrRollbackInfo {
    int narfcn = 0;
    int nrPci = 0;
    int nrRsrp = 0;
    int nrSinr = 0;
}
