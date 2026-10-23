/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable NwRateLimitingInfo {
    int rat = 0;
    int limitState = 0;
    int ambrUl = 0;
    int ambrDl = 0;
}
