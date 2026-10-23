/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable PlmnBlockInfo {
    int action = 0;
    int interval = 0;
    int mode = 0;
    int cause = 0;
    int mcc = 0;
    int mnc = 0;
    boolean pcs = false;
}
