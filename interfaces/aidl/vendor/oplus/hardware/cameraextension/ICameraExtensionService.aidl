/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.cameraextension;

@VintfStability
interface ICameraExtensionService {
    int preOpenCamera(int cameraId);
}
