/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable ProtInfo {
    byte cdma_prot_state;
    byte evdo_prot_state;
    char mac_index;
    byte[] sector_id;
    char drc;
    byte at_state;
    byte session_state;
    byte almp_state;
    byte init_state;
    byte idle_state;
    byte conn_state;
    byte rup_state;
    byte ovhd_state;
    byte hybrid_mode;
    byte trans_id;
    byte msg_seq;
    byte result;
    char duration;
    int success_count = 0;
    int failure_count = 0;
    int attempts_count = 0;
    char pn;
    byte[] sector_id2;
    byte color_code;
    byte num_ho;
}
