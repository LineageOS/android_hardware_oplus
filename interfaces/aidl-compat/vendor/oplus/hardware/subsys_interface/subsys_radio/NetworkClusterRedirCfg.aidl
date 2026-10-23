/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.NetworkClusterRedirRecord;

@VintfStability
parcelable NetworkClusterRedirCfg {
    char num;
    char mcc;
    byte month = 0;
    byte day = 0;
    char year;
    NetworkClusterRedirRecord[] redirData;
}
