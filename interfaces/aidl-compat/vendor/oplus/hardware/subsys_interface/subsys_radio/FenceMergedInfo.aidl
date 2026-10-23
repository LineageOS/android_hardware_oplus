/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable FenceMergedInfo {
    int fenceType = 0;
    int clientId = 0;
    int newFenceId = 0;
    int[] obsoleteFenceIds;
}
