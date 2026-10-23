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
