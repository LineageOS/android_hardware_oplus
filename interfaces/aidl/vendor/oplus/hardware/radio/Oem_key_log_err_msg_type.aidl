/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

@VintfStability
parcelable Oem_key_log_err_msg_type {
    char type;
    char rat;
    char errcode;
    char is_message;
    String msg;
}
