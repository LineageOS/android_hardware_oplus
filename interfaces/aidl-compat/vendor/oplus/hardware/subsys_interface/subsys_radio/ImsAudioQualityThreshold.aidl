/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable ImsAudioQualityThreshold {
    int enable = 0;
    int th_ul_pdcp_lost = 0;
    int th_dl_pdcp_lost = 0;
    int th_pdcp_buffer_low = 0;
    int th_pdcp_buffer_middle = 0;
    int th_pdcp_buffer_high = 0;
    int th_ul_bler_low = 0;
    int th_ul_bler_high = 0;
    int th_dl_bler_low = 0;
    int th_dl_bler_high = 0;
    int th_rsrp_low = 0;
    int th_rsrp_high = 0;
    int th_rsrq_low = 0;
    int th_rsrq_high = 0;
    int th_sinr_low = 0;
    int th_sinr_high = 0;
    int th_tb_size_low = 0;
    int th_tb_size_high = 0;
    int th_grant_use_percent_low = 0;
    int th_grant_use_percent_high = 0;
    int th_rtp_lost = 0;
    int th_rtp_jitter = 0;
}
