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
  void commonReq(int serial, in byte[] data, int datalen);
  void controlModemFeature(int serial, in int[] value);
  void get5GBandRequest(int serial);
  void get5GSignalfromCmapi(int serial, int rat, int DataId);
  void getASDIVState(int serial, int rat);
  void getBandMode(int serial);
  void getMdmBaseBand(int serial);
  void getMsimSubModeState(int serial);
  void getNrMode(int serial);
  void getNwSearchCount(int serial, int type);
  void getRadioInfo(int serial);
  void getRegionChangedForEccList(int serial);
  void getRfFeature(int serial);
  void getRffeCmd(int serial, in vendor.oplus.hardware.radio.OPLUS_rffe_data_type value);
  void getRffeDevInfo(int serial, int rf_tech);
  void getRilMcc(int serial, int slotId);
  void getTxRxInfo(int serial, int sys_mode);
  void getVoNrEnabled(int serial);
  void lockGsmArfcn(int serial, int arfcn1);
  void lockLteCell(int serial, int arfcn1, int arfcn2);
  void oemHookReq(int serial, in byte[] data);
  void performLteAcqScanReq(int serial);
  void plmnRatSelectionReq(int serial, in byte[] data, int datalen);
  void processFactoryModeNV(int serial, byte cmd);
  void sendOemCarrierCommand(int serial, int cmd_id, String cmd_data);
  void set5GBandRequest(int serial, in vendor.oplus.hardware.radio.Oplus_5g_band_type nsa_band, in vendor.oplus.hardware.radio.Oplus_5g_band_type sa_band, int duration);
  void setCallback(vendor.oplus.hardware.radio.IOplusRadioResponse responseCallback, vendor.oplus.hardware.radio.IOplusRadioIndication indicationCallback);
  void setEccList(int serial, in vendor.oplus.hardware.radio.EccEntryInfo[] eccEntryInfos);
  void setFactoryModeGPIO(int serial, int status, int num);
  void setFakesBsWeight(int serial, in int[] value);
  void setFilterArfcn(int serial, int arfcn1, int arfcn2);
  void setHsrModeForListen(int serial, int value);
  void setIgnorePsPaging(boolean enabled, int serial);
  void setLogIdForListen(int serial, int operateType, int logType, int logId, int subId);
  void setModemErrorFatal(int serial, int reasonType);
  void setNrMode(int serial, int mode);
  void setPplmnList(int serial, in byte[] value);
  void setVoNrEnabled(int serial, boolean enable);
  void setVolteFr1(int serial, int flags);
  void setVolteFr2(int serial, int flags, int rsrp_thresh, int fr2_rsrp, int rsrp_adj);
  void simlockReq(int serial, in byte[] data, int datalen);
  void updateRegionlockBlob(int serial, in byte[] data, int datalen);
  void updateRegionlockStatus(int serial, byte regionlock_operator, byte operation, byte data);
  void getRegionlockStatus(int serial);
  void setMsimSubModePref(int serial, int mode);
  void setDataStallParam(int serial, in vendor.oplus.hardware.radio.DataStallParam param);
  void setNfList(int serial, int size, in vendor.oplus.hardware.radio.NfList list);
  void getNfList(int serial);
  void setL2nrSelection(int serial, boolean state);
  void setLteDubiousCell(int serial, int size, in vendor.oplus.hardware.radio.DubCellInfo[] cell);
  void setNrDubiousCell(int serial, int size, in int[] scs, in vendor.oplus.hardware.radio.DubCellInfo[] cell);
}
