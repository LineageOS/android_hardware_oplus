/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.PlmnId;

@VintfStability
parcelable CellIdentitiesInfo {
    PlmnId plmn;
    int band = 0;
    int pci = 0;
    boolean arfcnInd = false;
    int arfcnOrFreq = 0;
    boolean cidInd = false;
    long cidOrCgi = 0;
    boolean scsValid = false;
    int nr5gScs = 0;
}
