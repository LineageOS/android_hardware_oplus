/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable NasSysInfo {
    int lte_reg_domain = 0;
    byte lte_rej_cause;
    byte voice_support_on_lte;
}
