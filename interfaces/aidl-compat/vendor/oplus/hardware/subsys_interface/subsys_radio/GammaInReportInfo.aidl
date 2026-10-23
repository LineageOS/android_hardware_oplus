/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.GammaInReportElement;

@VintfStability
parcelable GammaInReportInfo {
    int cpid = 0;
    int result = 0;
    GammaInReportElement[] gammaInReports;
}
