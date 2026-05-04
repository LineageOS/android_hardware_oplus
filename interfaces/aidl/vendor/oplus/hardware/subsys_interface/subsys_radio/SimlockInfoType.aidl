/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable SimlockInfoType {
    byte lockType;
    byte devicelockState;
    byte operatorId;
    int feature = 0;
    int lockInfo = 0;
}
