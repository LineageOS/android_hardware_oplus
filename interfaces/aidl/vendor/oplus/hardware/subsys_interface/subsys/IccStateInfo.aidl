/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys;

@VintfStability
parcelable IccStateInfo {
    byte state = 0;
    byte bypass = 0;
    byte modeNum = 0;
    byte[] modeState;
    byte[] modeErrCode;
}
