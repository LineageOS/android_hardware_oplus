/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.UnitTestResultEntry;

@VintfStability
parcelable UnitTestResults {
    int testServiceType = 0;
    UnitTestResultEntry[] results;
    boolean isSessionComplete = false;
}
