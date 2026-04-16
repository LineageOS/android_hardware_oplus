/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.camera.aon;

import vendor.oplus.hardware.camera.aon.ResultData;

@VintfStability
parcelable AONResult {
    int resultType;
    ResultData resultData;
}
