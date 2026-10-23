/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable PowerSavingInfo {
    int cgType = 0;
    int uaiType = 0;
    int op = 0;
    int uaiDl = 0;
    int uaiUl = 0;
    int delay = 0;
    int prohibitTimer = 0;
    int uaiCount = 0;
    int uaiGap = 0;
    int joinGap = 0;
}
