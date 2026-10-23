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

package vendor.oplus.hardware.a2dp_offload;
@Backing(type="int") @VintfStability
enum Lhdcv5QualityIndex {
  UNKNOWN = 0,
  QUALITY_LOW0 = 1,
  QUALITY_LOW1 = 2,
  QUALITY_LOW2 = 3,
  QUALITY_LOW3 = 4,
  QUALITY_LOW4 = 5,
  QUALITY_LOW = 6,
  QUALITY_MID = 7,
  QUALITY_HIGH = 8,
  QUALITY_HIGH1 = 9,
  QUALITY_HIGH2 = 10,
  QUALITY_HIGH3 = 11,
  QUALITY_HIGH4 = 12,
  QUALITY_HIGH5 = 13,
  QUALITY_ABR = 14,
  QUALITY_UNLIMIT = 15,
  QUALITY_CTRL_RESET_ABR = 128,
  QUALITY_CTRL_END = 129,
  QUALITY_INVALID = 130,
}
