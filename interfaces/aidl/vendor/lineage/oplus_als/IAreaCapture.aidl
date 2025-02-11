/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lineage.oplus_als;

import vendor.lineage.oplus_als.AreaRgbCaptureResult;

@VintfStability
interface IAreaCapture {
    AreaRgbCaptureResult getAreaBrightness();
}
