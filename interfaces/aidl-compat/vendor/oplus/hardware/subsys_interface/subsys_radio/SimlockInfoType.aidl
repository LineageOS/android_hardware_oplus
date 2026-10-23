/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable SimlockInfoType {
    byte lockType = 0;
    byte devicelockState = 0;
    byte operatorId = 0;
    int feature = 0;
    int lockInfo = 0;
}
