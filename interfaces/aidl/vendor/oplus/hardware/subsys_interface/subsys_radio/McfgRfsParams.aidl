/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable McfgRfsParams {
    int rfsParamsVer = 0;
    int discoverTimerTimeout = 0;
    byte retryMbnLoading;
    byte retryRemoteInaccessible;
    String rfsPathDiscover;
    String oemHwPathInfo;
    String oemSwPathInfo;
}
