/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
///////////////////////////////////////////////////////////////////////////////
// THIS FILE IS IMMUTABLE. DO NOT EDIT IN ANY CASE.                          //
///////////////////////////////////////////////////////////////////////////////

// This file is a snapshot of an AIDL file. Do not edit it manually. There are
// two cases:
// 1). this is a frozen version file - do not edit this in any case.
// 2). this is a 'current' file. If you make a backwards compatible change to
//     the interface (from the latest frozen version), the build system will
//     prompt you to update this file with `m <name>-update-api`.
//
// You must not make a backward incompatible change to any AIDL file built
// with the aidl_interface module type with versions property set. The module
// type is used to build AIDL files in a way that they can be used across
// independently updatable components of the system. If a device is shipped
// with such a backward incompatible change, it has a high risk of breaking
// later when a module using the interface is updated, e.g., Mainline modules.

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
