/*
 * Copyright (C) 2025 The LineageOS Project
 *
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
  void setModemErrorFatal(int serial, int errorType);
  void setVoNrEnabled(int serial, boolean enabled);
  void getVoNrEnabled(int serial);
  void setEccList(int serial, in vendor.oplus.hardware.radio.EccEntryInfo[] eccList);
  void setHsrModeForListen(int serial, int mode);
  void setLogIdForListen(int serial, int logId, int packetType, int logDataLen, int logData);
  void setSlowStartConfig(int serial, int config);
  void setCtVolteMode(int serial, int mode);
  void simlockReq(int serial, in byte[] data, int length);
  void updateRegionlockBlob(int serial, in byte[] blob, int length);
  void updateRegionlockStatus(int serial, byte status, byte subStatus, byte subSubStatus);
  void getRegionlockStatus(int serial);
  void setNwCongestionCfg(int serial, in byte[] cfg, int length);
  void setModemEsimStatus(int serial, boolean enabled);
  void setCallback(vendor.oplus.hardware.radio.IOplusRadioResponse radioResponse, vendor.oplus.hardware.radio.IOplusRadioIndication radioIndication);
  void setCarrierIdentifierIndEnable(int serial, boolean enabled);
  void setCarrierIdentifierCallback(vendor.oplus.hardware.radio.IOplusCarrierIdentifierResponse response, vendor.oplus.hardware.radio.IOplusCarrierIdentifierIndication indication);
  void getSimlockOperatorId(int serial);
  void getSimlockFeestate(int serial);
  void setSimlockFeestate(int serial, byte state);
  void getSimlockUnlockState(int serial);
  void setSimlockFactoryResetTime(int serial, in byte[] time, int length);
  void getSimlockFactoryResetTime(int serial);
  void setSimlockActivateTime(int serial, in byte[] time, int length);
  void getSimlockActivateTime(int serial);
  void getSimlockFeature(int serial);
  void getSimlockLockmark(int serial);
  void getSimlockDeviceLockState(int serial);
  void getSimlockDeviceLockinfo(int serial);
  void getSimlockVersionInfo(int serial);
  void getSimlockMaxRetry(int serial);
  void getSimlockCurrentRetry(int serial);
  void setPsDetachAttachAction(int serial, int action);
  void unlockRegionlock(int serial, in byte[] key, byte keyType);
  void updateRegionlockKey(int serial, in byte[] oldKey, in byte[] newKey);
  void getRegionlockSettingData(int serial);
  void setRegionlockSettingData(int serial, in byte[] data);
  void setSimlockOfflineLock(int serial, boolean enabled, int timeout, in int[] retryCount, long timestamp);
  void getSimlockOfflineLock(int serial);
}
