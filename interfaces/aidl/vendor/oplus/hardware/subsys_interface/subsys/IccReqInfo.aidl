/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys;

@VintfStability
parcelable IccReqInfo {
    int id = 0;
    byte[] data;
    int timeoutMs = 0;
}
