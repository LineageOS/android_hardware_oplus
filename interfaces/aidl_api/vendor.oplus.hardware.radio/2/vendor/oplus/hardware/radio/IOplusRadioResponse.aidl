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
interface IOplusRadioResponse {
  void setCallbackExtResponse();
  void setNrModeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void getNrModeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void setModemErrorFatalResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void setVoNrEnabledResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void getVoNrEnabledResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, boolean result);
  void setEccListResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, String result);
  void SetHsrModeForListeningResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void SetLogIdForListeningResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void setSlowStartResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void setCtVolteModeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void simlockReqResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] result);
  void updateRegionlockBlobResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  void updateRegionlockStatusResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  void getRegionlockStatusResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  void setNwCongestionCfgResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  void setModemEsimStatusResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void getSimlockOperatorIdResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void getSimlockFeestateResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void setSimlockFeestateResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void getSimlockUnlockStateResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void setSimlockFactoryResetTimeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void getSimlockFactoryResetTimeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  void setSimlockActivateTimeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void getSimlockActivateTimeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  void getSimlockFeatureResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] feature);
  void getSimlockLockMarkResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] lockmark);
  void getSimlockDeviceLockStateResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int lockstate);
  void getSimlockDeviceLockinfoResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] lockinfo);
  void getSimlockVersionInfoResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] version);
  void getSimlockMaxRetryResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void getSimlockCurrentRetryResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void setPsDetachAttachActionResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void unlockRegionlockResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void updateRegionlockKeyResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void getRegionlockSettingDataResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] result);
  void setRegionlockSettingDataResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void setSimlockOfflineLockResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void getSimlockOfflineLockResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in int[] retryCount);
}
