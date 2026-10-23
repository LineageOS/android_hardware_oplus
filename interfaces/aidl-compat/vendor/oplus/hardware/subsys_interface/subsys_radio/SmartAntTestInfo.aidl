/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.GammaInCaseInfo;

@VintfStability
parcelable SmartAntTestInfo {
    int mode = 0;
    int oslkTunerType = 0;
    int gammaInTestType = 0;
    GammaInCaseInfo[] gammaInCaseList;
}
