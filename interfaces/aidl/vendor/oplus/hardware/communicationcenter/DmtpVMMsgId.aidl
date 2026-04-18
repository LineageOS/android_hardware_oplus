/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.communicationcenter;

@Backing(type="int")
@VintfStability
enum DmtpVMMsgId {
    VM_CALL_START = 5000,
    VM_CALL_END = 5001,
    VN_CALL_MODIFY = 5002,
    VM_CALL_MODE_CHANGED = 5003,
}
