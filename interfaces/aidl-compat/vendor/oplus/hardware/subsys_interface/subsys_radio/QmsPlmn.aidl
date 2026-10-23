/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable QmsPlmn {
    int mcc = 0;
    int mnc = 0;
    boolean pcsDigit = false;
}
