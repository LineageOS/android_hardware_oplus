/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable McfgRfsParams {
    int rfsParamsVer = 0;
    int discoverTimerTimeout = 0;
    byte retryMbnLoading = 0;
    byte retryRemoteInaccessible = 0;
    String rfsPathDiscover;
    String oemHwPathInfo;
    String oemSwPathInfo;
}
