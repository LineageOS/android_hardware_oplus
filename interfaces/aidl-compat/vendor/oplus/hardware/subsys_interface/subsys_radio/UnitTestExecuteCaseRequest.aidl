/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable UnitTestExecuteCaseRequest {
    int testServiceType = 0;
    String[] caseName;
    int loopTimes = 0;
    int totalTimeoutMs = 0;
}
