/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.OplusRadioResponseInfo;

@VintfStability
interface IOplusRadioResponse {
    void setCallbackExtResponse();
    void setNrModeResponse(in OplusRadioResponseInfo info, int result);
    void getNrModeResponse(in OplusRadioResponseInfo info, int result);
    void setModemErrorFatalResponse(in OplusRadioResponseInfo info);
    void setVoNrEnabledResponse(in OplusRadioResponseInfo info);
    void getVoNrEnabledResponse(in OplusRadioResponseInfo info, boolean result);
    void setEccListResponse(in OplusRadioResponseInfo info, String result);
    void SetHsrModeForListeningResponse(in OplusRadioResponseInfo info, int result);
    void SetLogIdForListeningResponse(in OplusRadioResponseInfo info, int result);
    void setSlowStartResponse(in OplusRadioResponseInfo info);
    void setCtVolteModeResponse(in OplusRadioResponseInfo info);
    void simlockReqResponse(in OplusRadioResponseInfo info, in byte[] result);
    void updateRegionlockBlobResponse(in OplusRadioResponseInfo info, in byte[] status);
    void updateRegionlockStatusResponse(in OplusRadioResponseInfo info, in byte[] status);
    void getRegionlockStatusResponse(in OplusRadioResponseInfo info, in byte[] status);
    void setNwCongestionCfgResponse(in OplusRadioResponseInfo info, in byte[] status);
    void setModemEsimStatusResponse(in OplusRadioResponseInfo info);
    void getSimlockOperatorIdResponse(in OplusRadioResponseInfo info, int result);
    void getSimlockFeestateResponse(in OplusRadioResponseInfo info, int result);
    void setSimlockFeestateResponse(in OplusRadioResponseInfo info);
    void getSimlockUnlockStateResponse(in OplusRadioResponseInfo info, int result);
    void setSimlockFactoryResetTimeResponse(in OplusRadioResponseInfo info);
    void getSimlockFactoryResetTimeResponse(in OplusRadioResponseInfo info, in byte[] status);
    void setSimlockActivateTimeResponse(in OplusRadioResponseInfo info);
    void getSimlockActivateTimeResponse(in OplusRadioResponseInfo info, in byte[] status);
    void getSimlockFeatureResponse(in OplusRadioResponseInfo info, in byte[] feature);
    void getSimlockLockMarkResponse(in OplusRadioResponseInfo info, in byte[] lockmark);
    void getSimlockDeviceLockStateResponse(in OplusRadioResponseInfo info, int lockstate);
    void getSimlockDeviceLockinfoResponse(in OplusRadioResponseInfo info, in byte[] lockinfo);
    void getSimlockVersionInfoResponse(in OplusRadioResponseInfo info, in byte[] version);
    void getSimlockMaxRetryResponse(in OplusRadioResponseInfo info, int result);
    void getSimlockCurrentRetryResponse(in OplusRadioResponseInfo info, int result);
    void setPsDetachAttachActionResponse(in OplusRadioResponseInfo info);
    void unlockRegionlockResponse(in OplusRadioResponseInfo info, int result);
    void updateRegionlockKeyResponse(in OplusRadioResponseInfo info, int result);
    void getRegionlockSettingDataResponse(in OplusRadioResponseInfo info, in byte[] result);
    void setRegionlockSettingDataResponse(in OplusRadioResponseInfo info, int result);
}
