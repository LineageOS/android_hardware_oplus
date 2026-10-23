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
  oneway void setNrMode(int serial, int mode);
  oneway void getNrMode(int serial);
  oneway void setModemErrorFatal(int serial, int reasonType);
  oneway void setVoNrEnabled(int serial, boolean enable);
  oneway void getVoNrEnabled(int serial);
  oneway void setEccList(int serial, in vendor.oplus.hardware.radio.EccEntryInfo[] eccEntryInfos);
  oneway void setHsrModeForListen(int serial, int value);
  oneway void setLogIdForListen(int serial, int operateType, int logType, int logId, int subId);
  oneway void setSlowStartConfig(int serial, int percent);
  oneway void setCtVolteMode(int serial, int volteMode);
  oneway void simlockReq(int serial, in byte[] data, int datalen);
  oneway void updateRegionlockBlob(int serial, in byte[] data, int datalen);
  oneway void updateRegionlockStatus(int serial, byte regionlock_operator, byte operation, byte data);
  oneway void getRegionlockStatus(int serial);
  oneway void setNwCongestionCfg(int serial, in byte[] data, int datalen);
  oneway void setModemEsimStatus(int serial, boolean status);
  void setCallback(in vendor.oplus.hardware.radio.IOplusRadioResponse responseCallback, in vendor.oplus.hardware.radio.IOplusRadioIndication indicationCallback);
  oneway void setCarrierIdentifierIndEnable(int serial, boolean enable);
  oneway void setCarrierIdentifierCallback(in vendor.oplus.hardware.radio.IOplusCarrierIdentifierResponse respCallback, in vendor.oplus.hardware.radio.IOplusCarrierIdentifierIndication indCallback);
  oneway void getSimlockOperatorId(int serial);
  oneway void getSimlockFeestate(int serial);
  oneway void setSimlockFeestate(int serial, byte feestate);
  oneway void getSimlockUnlockState(int serial);
  oneway void setSimlockFactoryResetTime(int serial, in byte[] data, int datalen);
  oneway void getSimlockFactoryResetTime(int serial);
  oneway void setSimlockActivateTime(int serial, in byte[] data, int datalen);
  oneway void getSimlockActivateTime(int serial);
  oneway void getSimlockFeature(int serial);
  oneway void getSimlockLockmark(int serial);
  oneway void getSimlockDeviceLockState(int serial);
  oneway void getSimlockDeviceLockinfo(int serial);
  oneway void getSimlockVersionInfo(int serial);
  oneway void getSimlockMaxRetry(int serial);
  oneway void getSimlockCurrentRetry(int serial);
  oneway void setPsDetachAttachAction(int serial, int action);
  oneway void unlockRegionlock(int serial, in byte[] data, byte mode);
  oneway void updateRegionlockKey(int serial, in byte[] data, in byte[] key);
  oneway void getRegionlockSettingData(int serial);
  oneway void setRegionlockSettingData(int serial, in byte[] data);
  oneway void setSimlockOfflineLock(int serial, boolean enabled, int timeout, in int[] retryCount, long timestamp);
  oneway void getSimlockOfflineLock(int serial);
  oneway void setLlmMode(int serial, int mode);
}
