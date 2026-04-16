/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.cammidasservice;

import vendor.oplus.hardware.cammidasservice.SensorDimension;

@VintfStability
parcelable CameraInfo {
    int physicalCameraId;
    int isActive;
    SensorDimension sensorOutDimension;
}
