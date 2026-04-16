/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.camera.aon;

import vendor.oplus.hardware.camera.aon.AONResult;

@VintfStability
parcelable AONCallBackInfo {
    int serviceType;
    AONResult[] results;
}
