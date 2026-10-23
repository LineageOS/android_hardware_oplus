/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable ActionResult {
    int clientId = 0;
    int actionId = 0;
    int firstLevel = 0;
    int finalLevel = 0;
    int result = 0;
    int startTimeMs = 0;
    int finishTimeMs = 0;
}
