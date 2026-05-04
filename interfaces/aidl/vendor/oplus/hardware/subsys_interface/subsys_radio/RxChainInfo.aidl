/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable RxChainInfo {
    byte isRadioTurned;
    int rxPwr = 0;
    int ecio = 0;
    int rscp = 0;
    int rsrp = 0;
    int phase = 0;
}
