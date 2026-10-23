/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable HplmnBgSearchInfo {
    int rat = 0;
    int cmd = 0;
    int repeat_interval = 0;
    int work_time = 0;
    int reserved = 0;
}
