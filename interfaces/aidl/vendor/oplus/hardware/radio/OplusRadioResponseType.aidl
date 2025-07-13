/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

@VintfStability
@Backing(type="int")
enum OplusRadioResponseType {
    SOLICITED = 0,
    SOLICITED_ACK = 1,
    SOLICITED_ACK_EXP = 2
}
