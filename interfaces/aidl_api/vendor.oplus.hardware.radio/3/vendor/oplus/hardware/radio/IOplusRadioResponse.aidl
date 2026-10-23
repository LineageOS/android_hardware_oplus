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
  oneway void setCallbackExtResponse();
  oneway void setNrModeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  oneway void getNrModeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  oneway void setModemErrorFatalResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  oneway void setVoNrEnabledResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  oneway void getVoNrEnabledResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, boolean result);
  oneway void setEccListResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, String result);
  oneway void SetHsrModeForListeningResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  oneway void SetLogIdForListeningResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  oneway void setSlowStartResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  oneway void setCtVolteModeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  oneway void simlockReqResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] result);
  oneway void updateRegionlockBlobResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  oneway void updateRegionlockStatusResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  oneway void getRegionlockStatusResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  oneway void setNwCongestionCfgResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  oneway void setModemEsimStatusResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  oneway void getSimlockOperatorIdResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  oneway void getSimlockFeestateResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  oneway void setSimlockFeestateResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  oneway void getSimlockUnlockStateResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  oneway void setSimlockFactoryResetTimeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  oneway void getSimlockFactoryResetTimeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  oneway void setSimlockActivateTimeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  oneway void getSimlockActivateTimeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  oneway void getSimlockFeatureResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] feature);
  oneway void getSimlockLockMarkResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] lockmark);
  oneway void getSimlockDeviceLockStateResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int lockstate);
  oneway void getSimlockDeviceLockinfoResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] lockinfo);
  oneway void getSimlockVersionInfoResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] version);
  oneway void getSimlockMaxRetryResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  oneway void getSimlockCurrentRetryResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  oneway void setPsDetachAttachActionResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  oneway void unlockRegionlockResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  oneway void updateRegionlockKeyResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  oneway void getRegionlockSettingDataResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] result);
  oneway void setRegionlockSettingDataResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  oneway void setSimlockOfflineLockResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  oneway void getSimlockOfflineLockResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in int[] retryCount);
  oneway void setLlmModeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
}
