/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable GammaInReportElement {
    int freq = 0;
    int evm = 0;
    int gammaInI = 0;
    int gammaInQ = 0;
}
