/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@Backing(type="int") @VintfStability
enum ERadioTech {
    CDMA = 1,
    GSM = 0,
    LTE = 4,
    SUB6 = 5,
    TDSCDMA = 3,
    WCDMA = 2,
}
