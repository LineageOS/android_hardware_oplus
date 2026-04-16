/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.sendextcamcmd;

@VintfStability
parcelable ExtCamCmd {
    int cmd;
    int[] data;
}
