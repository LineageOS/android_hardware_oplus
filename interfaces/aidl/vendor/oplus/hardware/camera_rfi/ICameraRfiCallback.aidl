/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.camera_rfi;

import vendor.oplus.hardware.camera_rfi.CameraRfiIndicationType;
import vendor.oplus.hardware.camera_rfi.CameraRfiStatus;

@VintfStability
interface ICameraRfiCallback {
    void OnRfiStatusChange(CameraRfiIndicationType indType, CameraRfiStatus newStatus);
}
