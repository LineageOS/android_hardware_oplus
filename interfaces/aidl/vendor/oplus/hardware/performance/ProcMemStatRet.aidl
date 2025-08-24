/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.performance;

import vendor.oplus.hardware.performance.ProcMemStat;

@VintfStability
parcelable ProcMemStatRet {
    int status;
    ProcMemStat[] arr_ms;
}
