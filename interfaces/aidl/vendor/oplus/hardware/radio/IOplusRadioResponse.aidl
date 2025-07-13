/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.OplusRadioResponseInfo;

@VintfStability
interface IOplusRadioResponse {
    void setCallbackExtResponse();
    void setNrModeResponse(in OplusRadioResponseInfo info, int mode);
    void getNrModeResponse(in OplusRadioResponseInfo info, int mode);
    void setModemErrorFatalResponse(in OplusRadioResponseInfo info);
    void setVoNrEnabledResponse(in OplusRadioResponseInfo info);
    void getVoNrEnabledResponse(in OplusRadioResponseInfo info, boolean enabled);
    void setEccListResponse(in OplusRadioResponseInfo info, String result);
    void SetHsrModeForListeningResponse(in OplusRadioResponseInfo info, int mode);
    void SetLogIdForListeningResponse(in OplusRadioResponseInfo info, int logId);
    void setSlowStartResponse(in OplusRadioResponseInfo info);
    void setCtVolteModeResponse(in OplusRadioResponseInfo info);
    void simlockReqResponse(in OplusRadioResponseInfo info, in byte[] data);
    void updateRegionlockBlobResponse(in OplusRadioResponseInfo info, in byte[] blob);
    void updateRegionlockStatusResponse(in OplusRadioResponseInfo info, in byte[] status);
    void getRegionlockStatusResponse(in OplusRadioResponseInfo info, in byte[] status);
    void setNwCongestionCfgResponse(in OplusRadioResponseInfo info, in byte[] cfg);
    void setModemEsimStatusResponse(in OplusRadioResponseInfo info);
    void getSimlockOperatorIdResponse(in OplusRadioResponseInfo info, int operatorId);
    void getSimlockFeestateResponse(in OplusRadioResponseInfo info, int state);
    void setSimlockFeestateResponse(in OplusRadioResponseInfo info);
    void getSimlockUnlockStateResponse(in OplusRadioResponseInfo info, int state);
    void setSimlockFactoryResetTimeResponse(in OplusRadioResponseInfo info);
    void getSimlockFactoryResetTimeResponse(in OplusRadioResponseInfo info, in byte[] time);
    void setSimlockActivateTimeResponse(in OplusRadioResponseInfo info);
    void getSimlockActivateTimeResponse(in OplusRadioResponseInfo info, in byte[] time);
    void getSimlockFeatureResponse(in OplusRadioResponseInfo info, in byte[] feature);
    void getSimlockLockMarkResponse(in OplusRadioResponseInfo info, in byte[] lockMark);
    void getSimlockDeviceLockStateResponse(in OplusRadioResponseInfo info, int state);
    void getSimlockDeviceLockinfoResponse(in OplusRadioResponseInfo info, in byte[] lockInfo);
    void getSimlockVersionInfoResponse(in OplusRadioResponseInfo info, in byte[] version);
    void getSimlockMaxRetryResponse(in OplusRadioResponseInfo info, int maxRetry);
    void getSimlockCurrentRetryResponse(in OplusRadioResponseInfo info, int currentRetry);
    void setPsDetachAttachActionResponse(in OplusRadioResponseInfo info);
    void unlockRegionlockResponse(in OplusRadioResponseInfo info, int result);
    void updateRegionlockKeyResponse(in OplusRadioResponseInfo info, int result);
    void getRegionlockSettingDataResponse(in OplusRadioResponseInfo info, in byte[] data);
    void setRegionlockSettingDataResponse(in OplusRadioResponseInfo info, int result);
    void setSimlockOfflineLockResponse(in OplusRadioResponseInfo info);
    void getSimlockOfflineLockResponse(in OplusRadioResponseInfo info, in int[] retryCount);
}
