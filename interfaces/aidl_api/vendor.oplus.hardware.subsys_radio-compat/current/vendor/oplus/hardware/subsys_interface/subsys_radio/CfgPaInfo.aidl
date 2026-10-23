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
parcelable CfgPaInfo {
  int onOff = 0;
  int techType = 0;
  int band = 0;
  int rfMode = 0;
  int regAddr0 = 0;
  int regAddr1 = 0;
  int regAddr2 = 0;
  int addr0Range0 = 0;
  int addr0Range1 = 0;
  int addr0Range2 = 0;
  int addr0Range3 = 0;
  int addr1Range0 = 0;
  int addr1Range1 = 0;
  int addr1Range2 = 0;
  int addr1Range3 = 0;
  int addr2Range0 = 0;
  int addr2Range1 = 0;
  int addr2Range2 = 0;
  int addr2Range3 = 0;
}
