/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.OplusRadioResponseInfo;

@VintfStability
interface IOplusRadioResponse {
    oneway void setCallbackExtResponse();
    oneway void setNrModeResponse(in OplusRadioResponseInfo info, int result);
    oneway void getNrModeResponse(in OplusRadioResponseInfo info, int result);
    oneway void setModemErrorFatalResponse(in OplusRadioResponseInfo info);
    oneway void setVoNrEnabledResponse(in OplusRadioResponseInfo info);
    oneway void getVoNrEnabledResponse(in OplusRadioResponseInfo info, boolean result);
    oneway void setEccListResponse(in OplusRadioResponseInfo info, String result);
    oneway void SetHsrModeForListeningResponse(in OplusRadioResponseInfo info, int result);
    oneway void SetLogIdForListeningResponse(in OplusRadioResponseInfo info, int result);
    oneway void setSlowStartResponse(in OplusRadioResponseInfo info);
    oneway void setCtVolteModeResponse(in OplusRadioResponseInfo info);
    oneway void simlockReqResponse(in OplusRadioResponseInfo info, in byte[] result);
    oneway void updateRegionlockBlobResponse(in OplusRadioResponseInfo info, in byte[] status);
    oneway void updateRegionlockStatusResponse(in OplusRadioResponseInfo info, in byte[] status);
    oneway void getRegionlockStatusResponse(in OplusRadioResponseInfo info, in byte[] status);
    oneway void setNwCongestionCfgResponse(in OplusRadioResponseInfo info, in byte[] status);
    oneway void setModemEsimStatusResponse(in OplusRadioResponseInfo info);
    oneway void getSimlockOperatorIdResponse(in OplusRadioResponseInfo info, int result);
    oneway void getSimlockFeestateResponse(in OplusRadioResponseInfo info, int result);
    oneway void setSimlockFeestateResponse(in OplusRadioResponseInfo info);
    oneway void getSimlockUnlockStateResponse(in OplusRadioResponseInfo info, int result);
    oneway void setSimlockFactoryResetTimeResponse(in OplusRadioResponseInfo info);
    oneway void getSimlockFactoryResetTimeResponse(in OplusRadioResponseInfo info, in byte[] status);
    oneway void setSimlockActivateTimeResponse(in OplusRadioResponseInfo info);
    oneway void getSimlockActivateTimeResponse(in OplusRadioResponseInfo info, in byte[] status);
    oneway void getSimlockFeatureResponse(in OplusRadioResponseInfo info, in byte[] feature);
    oneway void getSimlockLockMarkResponse(in OplusRadioResponseInfo info, in byte[] lockmark);
    oneway void getSimlockDeviceLockStateResponse(in OplusRadioResponseInfo info, int lockstate);
    oneway void getSimlockDeviceLockinfoResponse(in OplusRadioResponseInfo info, in byte[] lockinfo);
    oneway void getSimlockVersionInfoResponse(in OplusRadioResponseInfo info, in byte[] version);
    oneway void getSimlockMaxRetryResponse(in OplusRadioResponseInfo info, int result);
    oneway void getSimlockCurrentRetryResponse(in OplusRadioResponseInfo info, int result);
    oneway void setPsDetachAttachActionResponse(in OplusRadioResponseInfo info);
    oneway void unlockRegionlockResponse(in OplusRadioResponseInfo info, int result);
    oneway void updateRegionlockKeyResponse(in OplusRadioResponseInfo info, int result);
    oneway void getRegionlockSettingDataResponse(in OplusRadioResponseInfo info, in byte[] result);
    oneway void setRegionlockSettingDataResponse(in OplusRadioResponseInfo info, int result);
    oneway void setSimlockOfflineLockResponse(in OplusRadioResponseInfo info);
    oneway void getSimlockOfflineLockResponse(in OplusRadioResponseInfo info, in int[] retryCount);
}
