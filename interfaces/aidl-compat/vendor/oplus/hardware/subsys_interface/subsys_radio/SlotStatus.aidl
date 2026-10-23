/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable SlotStatus {
    byte cardState = 0;
    byte slotState = 0;
    byte logicalSlot = 0;
    byte iccid_len = 0;
    byte[] iccid;
}
