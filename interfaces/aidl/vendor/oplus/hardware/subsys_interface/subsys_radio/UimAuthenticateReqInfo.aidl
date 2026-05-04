/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable UimAuthenticateReqInfo {
    int session_type = 0;
    char aid_len;
    byte[] aid;
    char context;
    char authdata_len;
    byte[] authdata;
}
