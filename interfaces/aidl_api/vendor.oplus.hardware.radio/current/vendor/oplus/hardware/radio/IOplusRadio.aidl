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
interface IOplusRadio {
  void setNrMode(int serial, int mode);
  void getNrMode(int serial);
  void setModemErrorFatal(int serial, int reasonType);
  void setVoNrEnabled(int serial, boolean enable);
  void getVoNrEnabled(int serial);
  void setEccList(int serial, in vendor.oplus.hardware.radio.EccEntryInfo[] eccEntryInfos);
  void setHsrModeForListen(int serial, int value);
  void setLogIdForListen(int serial, int operateType, int logType, int logId, int subId);
  void setSlowStartConfig(int serial, int percent);
  void setCtVolteMode(int serial, int volteMode);
  void simlockReq(int serial, in byte[] data, int datalen);
  void updateRegionlockBlob(int serial, in byte[] data, int datalen);
  void updateRegionlockStatus(int serial, byte regionlock_operator, byte operation, byte data);
  void getRegionlockStatus(int serial);
  void setNwCongestionCfg(int serial, in byte[] data, int datalen);
  void setModemEsimStatus(int serial, boolean status);
  void setCallback(in vendor.oplus.hardware.radio.IOplusRadioResponse responseCallback, in vendor.oplus.hardware.radio.IOplusRadioIndication indicationCallback);
  void setCarrierIdentifierIndEnable(int serial, boolean enable);
  void setCarrierIdentifierCallback(in vendor.oplus.hardware.radio.IOplusCarrierIdentifierResponse respCallback, in vendor.oplus.hardware.radio.IOplusCarrierIdentifierIndication indCallback);
  void getSimlockOperatorId(int serial);
  void getSimlockFeestate(int serial);
  void setSimlockFeestate(int serial, byte feestate);
  void getSimlockUnlockState(int serial);
  void setSimlockFactoryResetTime(int serial, in byte[] data, int datalen);
  void getSimlockFactoryResetTime(int serial);
  void setSimlockActivateTime(int serial, in byte[] data, int datalen);
  void getSimlockActivateTime(int serial);
  void getSimlockFeature(int serial);
  void getSimlockLockmark(int serial);
  void getSimlockDeviceLockState(int serial);
  void getSimlockDeviceLockinfo(int serial);
  void getSimlockVersionInfo(int serial);
  void getSimlockMaxRetry(int serial);
  void getSimlockCurrentRetry(int serial);
  void setPsDetachAttachAction(int serial, int action);
  void unlockRegionlock(int serial, in byte[] data, byte mode);
  void updateRegionlockKey(int serial, in byte[] data, in byte[] key);
  void getRegionlockSettingData(int serial);
  void setRegionlockSettingData(int serial, in byte[] data);
}
