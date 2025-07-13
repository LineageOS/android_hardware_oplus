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
interface IOplusRadioResponse {
  void SetHsrModeForListeningResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void SetLogIdForListeningResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void commonReqResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] result);
  void controlModemFeatureResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void get5GBandResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in vendor.oplus.hardware.radio.Oplus_5g_band_type nsa_band, in vendor.oplus.hardware.radio.Oplus_5g_band_type sa_band, int cmd_err);
  void getASDIVStateResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, String result);
  void getMdmBaseBandResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, String baseband);
  void getMsimSubModeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int msim_sub_mode, int dsda_tx_mode, int cmd_err);
  void getNrModeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void getNwSearchCountResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in int[] data);
  void getRadioInfoResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in vendor.oplus.hardware.radio.OPLUS_RIL_Radio_info respInfo);
  void getRegionChangedForEccListResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void getRfFeatureResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, String result);
  void getRffeCmdResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void getRffeDevInfoResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void getRilMccResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, String result);
  void getTxRxInfoResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in vendor.oplus.hardware.radio.OPLUS_RIL_Tx_Rx_info result);
  void getVoNrEnabledResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, boolean result);
  void get_fiveG_signal_from_Cmapi_Response(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in vendor.oplus.hardware.radio.OPLUS_cmapi_signal response);
  void lockGsmArfcnResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void lockLteCellResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void oemHookResp(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] data, int cmd);
  void performLteAcqScanResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int cmd_res);
  void plmnRatSelectionResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] result);
  void processFactoryModeNVResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void reportNvRestoreResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void reserveNullResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void sendOemCarrierCommandResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, String result);
  void set5GBandResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int cmd_err);
  void setCallbackExtResponse();
  void setEccListResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, String result);
  void setFactoryModeGPIOResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void setFakesBsWeightResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void setFilterArfcnResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void setIgnorePsPagingResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void setModemErrorFatalResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void setNrModeResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void setPplmnListResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void setVoNrEnabledResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void setVolteFr1Response(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void setVolteFr2Response(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int result);
  void simlockReqResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] result);
  void updateRegionlockBlobResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  void updateRegionlockStatusResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  void getRegionlockStatusResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, in byte[] status);
  void setMsimSubModePrefResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info, int cmd_res);
  void setDataStallParamResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void setNfListResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void getNfListResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void setL2nrSelectionResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void setLteDubCellInfoResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
  void setNrDubCellInfoResponse(in vendor.oplus.hardware.radio.OplusRadioResponseInfo info);
}
