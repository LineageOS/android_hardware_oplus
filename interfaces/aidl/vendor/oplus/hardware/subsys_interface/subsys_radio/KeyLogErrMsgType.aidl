/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable KeyLogErrMsgType {
    char type;
    char rat;
    char errcode;
    char isMessage;
    String msg;
}
