/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys;

@VintfStability
@Backing(type="int")
enum SubsysResponseType {
    SOLICITED = 0,
    SOLICITED_ACK = 1,
    SOLICITED_ACK_EXP = 2,
}
