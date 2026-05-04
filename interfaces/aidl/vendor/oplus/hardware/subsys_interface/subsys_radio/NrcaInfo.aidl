/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.Nr5gMl1Dlm2CarrierConfig;

@VintfStability
parcelable NrcaInfo {
    byte numCarriers;
    Nr5gMl1Dlm2CarrierConfig[] carrierConfig;
}
