/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.QmsPlmn;

@VintfStability
parcelable NfList {
    int arfcn = 0;
    int band = 0;
    byte scs;
    int plmnSize = 0;
    QmsPlmn[] plmn;
}
