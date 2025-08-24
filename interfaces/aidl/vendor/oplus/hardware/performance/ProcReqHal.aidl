/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.performance;

import vendor.oplus.hardware.performance.ProcReqItemHal;

@VintfStability
parcelable ProcReqHal {
    String proc;
    ProcReqItemHal[] items;
}
