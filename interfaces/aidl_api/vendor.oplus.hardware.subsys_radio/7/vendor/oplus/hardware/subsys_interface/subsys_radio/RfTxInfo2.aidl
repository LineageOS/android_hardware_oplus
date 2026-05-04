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
parcelable RfTxInfo2 {
  int radioTech = 0;
  int band = 0;
  int bandwidth = 0;
  int ulChannel = 0;
  int dlChannel = 0;
  int dbm = 0;
  int onOff = 0;
  int ant = 0;
  int startRb = 0;
  int rbNumber = 0;
  int subband = 0;
  int paIndex = 0;
  int dutycycle = 0;
  int waveform = 0;
  int modulation = 0;
  int scs = 0;
  int txRoute = 0;
  int txState = 0;
  int rxState = 0;
  int txParam1 = 0;
  int txParam2 = 0;
  int txParam3 = 0;
  int txParam4 = 0;
  int txParam5 = 0;
}
