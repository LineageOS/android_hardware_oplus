/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@Backing(type="int")
@VintfStability
enum ERadioTech {
    GSM = 0,
    CDMA = 1,
    WCDMA = 2,
    TDSCDMA = 3,
    LTE = 4,
    SUB6 = 5,
}
