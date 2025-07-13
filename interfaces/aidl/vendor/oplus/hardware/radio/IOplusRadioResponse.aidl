/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.OPLUS_RIL_Radio_info;
import vendor.oplus.hardware.radio.OPLUS_RIL_Tx_Rx_info;
import vendor.oplus.hardware.radio.OPLUS_cmapi_signal;
import vendor.oplus.hardware.radio.OplusRadioResponseInfo;
import vendor.oplus.hardware.radio.Oplus_5g_band_type;

@VintfStability
interface IOplusRadioResponse {
    void SetHsrModeForListeningResponse(in OplusRadioResponseInfo info, int result);
    void SetLogIdForListeningResponse(in OplusRadioResponseInfo info, int result);
    void commonReqResponse(in OplusRadioResponseInfo info, in byte[] result);
    void controlModemFeatureResponse(in OplusRadioResponseInfo info, int result);
    void get5GBandResponse(in OplusRadioResponseInfo info, in Oplus_5g_band_type nsa_band, in Oplus_5g_band_type sa_band, int cmd_err);
    void getASDIVStateResponse(in OplusRadioResponseInfo info, String result);
    void getMdmBaseBandResponse(in OplusRadioResponseInfo info, String baseband);
    void getMsimSubModeResponse(in OplusRadioResponseInfo info, int msim_sub_mode, int dsda_tx_mode, int cmd_err);
    void getNrModeResponse(in OplusRadioResponseInfo info, int result);
    void getNwSearchCountResponse(in OplusRadioResponseInfo info, in int[] data);
    void getRadioInfoResponse(in OplusRadioResponseInfo info, in OPLUS_RIL_Radio_info respInfo);
    void getRegionChangedForEccListResponse(in OplusRadioResponseInfo info);
    void getRfFeatureResponse(in OplusRadioResponseInfo info, String result);
    void getRffeCmdResponse(in OplusRadioResponseInfo info, int result);
    void getRffeDevInfoResponse(in OplusRadioResponseInfo info, int result);
    void getRilMccResponse(in OplusRadioResponseInfo info, String result);
    void getTxRxInfoResponse(in OplusRadioResponseInfo info, in OPLUS_RIL_Tx_Rx_info result);
    void getVoNrEnabledResponse(in OplusRadioResponseInfo info, boolean result);
    void get_fiveG_signal_from_Cmapi_Response(in OplusRadioResponseInfo info, in OPLUS_cmapi_signal response);
    void lockGsmArfcnResponse(in OplusRadioResponseInfo info, int result);
    void lockLteCellResponse(in OplusRadioResponseInfo info, int result);
    void oemHookResp(in OplusRadioResponseInfo info, in byte[] data, int cmd);
    void performLteAcqScanResponse(in OplusRadioResponseInfo info, int cmd_res);
    void plmnRatSelectionResponse(in OplusRadioResponseInfo info, in byte[] result);
    void processFactoryModeNVResponse(in OplusRadioResponseInfo info);
    void reportNvRestoreResponse(in OplusRadioResponseInfo info);
    void reserveNullResponse(in OplusRadioResponseInfo info);
    void sendOemCarrierCommandResponse(in OplusRadioResponseInfo info, String result);
    void set5GBandResponse(in OplusRadioResponseInfo info, int cmd_err);
    void setCallbackExtResponse();
    void setEccListResponse(in OplusRadioResponseInfo info, String result);
    void setFactoryModeGPIOResponse(in OplusRadioResponseInfo info, int result);
    void setFakesBsWeightResponse(in OplusRadioResponseInfo info, int result);
    void setFilterArfcnResponse(in OplusRadioResponseInfo info, int result);
    void setIgnorePsPagingResponse(in OplusRadioResponseInfo info);
    void setModemErrorFatalResponse(in OplusRadioResponseInfo info);
    void setNrModeResponse(in OplusRadioResponseInfo info, int result);
    void setPplmnListResponse(in OplusRadioResponseInfo info);
    void setVoNrEnabledResponse(in OplusRadioResponseInfo info);
    void setVolteFr1Response(in OplusRadioResponseInfo info, int result);
    void setVolteFr2Response(in OplusRadioResponseInfo info, int result);
    void simlockReqResponse(in OplusRadioResponseInfo info, in byte[] result);
    void updateRegionlockBlobResponse(in OplusRadioResponseInfo info, in byte[] status);
    void updateRegionlockStatusResponse(in OplusRadioResponseInfo info, in byte[] status);
    void getRegionlockStatusResponse(in OplusRadioResponseInfo info, in byte[] status);
    void setMsimSubModePrefResponse(in OplusRadioResponseInfo info, int cmd_res);
    void setDataStallParamResponse(in OplusRadioResponseInfo info);
    void setNfListResponse(in OplusRadioResponseInfo info);
    void getNfListResponse(in OplusRadioResponseInfo info);
    void setL2nrSelectionResponse(in OplusRadioResponseInfo info);
    void setLteDubCellInfoResponse(in OplusRadioResponseInfo info);
    void setNrDubCellInfoResponse(in OplusRadioResponseInfo info);
}
