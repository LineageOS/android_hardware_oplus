/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable PlmnRatInfo {
    int mcc = 0;
    int mnc = 0;
    boolean psc = false;
    int rat = 0;
    int regPref = 0;
    int netSelMode = 0;
}
