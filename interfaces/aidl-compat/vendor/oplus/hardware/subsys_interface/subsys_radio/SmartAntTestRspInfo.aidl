/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.GammaInReportInfo;

@VintfStability
parcelable SmartAntTestRspInfo {
    int mode = 0;
    int result = 0;
    int oslkFailCause = 0;
    int gammaInTestType = 0;
    int gammaInFailAntMap = 0;
    GammaInReportInfo[] gammaInReports;
}
