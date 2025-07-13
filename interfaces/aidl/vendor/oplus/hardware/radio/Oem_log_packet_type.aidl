/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

@VintfStability
parcelable Oem_log_packet_type {
    int packet_type;
    int log_id;
    int log_data_len;
    byte[] log_data;
}
