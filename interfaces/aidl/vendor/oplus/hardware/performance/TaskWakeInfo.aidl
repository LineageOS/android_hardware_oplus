/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.performance;

import vendor.oplus.hardware.performance.ThreadInfo;

@VintfStability
parcelable TaskWakeInfo {
    int ret = 0;
    ThreadInfo[] list;
}
