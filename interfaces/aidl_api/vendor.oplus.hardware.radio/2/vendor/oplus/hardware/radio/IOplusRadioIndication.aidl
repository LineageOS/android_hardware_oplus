/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
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

package vendor.oplus.hardware.radio;
@VintfStability
interface IOplusRadioIndication {
  void networkInfoInd(int type, in String[] networkinfo);
  void oemHotswapProcessInd(int type, int slotid, int error);
  void oemHotswapSetInd(int type, int slotid);
  void oemKeyLogErrInd(int type, in vendor.oplus.hardware.radio.Oem_key_log_err_msg_type errMsg);
  void oemLargeDataKeyLogErrInd(int type, in int[] result);
  void oemLogPacketInd(int type, in vendor.oplus.hardware.radio.Oem_log_packet_type log_packet_msg);
  void oemPinPukRetryNumInd(int type, int slotid, int pin_retry_num, int puk_retry_num);
  void smlDeviceLockInfoChangedInd(int type, String info);
  void smlEncryptedSerialIdInd(int type, in String[] indStgs);
  void subsidyDeviceLockInfoChangedInd(int type, String info);
  void regionlockStatusChangedInd(int type, in byte[] region_lock_status_info);
  void nrEvolutionTypeIndication(int type, int toe);
  void singleSimConfigChangedInd(int type, int result);
  void offlineLockStatusChangedInd(int type, in int[] status);
}
