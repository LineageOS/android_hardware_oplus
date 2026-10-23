/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable ProtInfo {
    byte cdma_prot_state = 0;
    byte evdo_prot_state = 0;
    char mac_index;
    byte[] sector_id;
    char drc;
    byte at_state = 0;
    byte session_state = 0;
    byte almp_state = 0;
    byte init_state = 0;
    byte idle_state = 0;
    byte conn_state = 0;
    byte rup_state = 0;
    byte ovhd_state = 0;
    byte hybrid_mode = 0;
    byte trans_id = 0;
    byte msg_seq = 0;
    byte result = 0;
    char duration;
    int success_count = 0;
    int failure_count = 0;
    int attempts_count = 0;
    char pn;
    byte[] sector_id2;
    byte color_code = 0;
    byte num_ho = 0;
}
