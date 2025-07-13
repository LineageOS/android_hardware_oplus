/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.EccEntryInfo;
import vendor.oplus.hardware.radio.IOplusCarrierIdentifierIndication;
import vendor.oplus.hardware.radio.IOplusCarrierIdentifierResponse;
import vendor.oplus.hardware.radio.IOplusRadioIndication;
import vendor.oplus.hardware.radio.IOplusRadioResponse;

@VintfStability
interface IOplusRadio {
    void setNrMode(int serial, int mode);
    void getNrMode(int serial);
    void setModemErrorFatal(int serial, int errorType);
    void setVoNrEnabled(int serial, boolean enabled);
    void getVoNrEnabled(int serial);
    void setEccList(int serial, in EccEntryInfo[] eccList);
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
    void setCallback(IOplusRadioResponse radioResponse, IOplusRadioIndication radioIndication);
    void setCarrierIdentifierIndEnable(int serial, boolean enabled);
    void setCarrierIdentifierCallback(IOplusCarrierIdentifierResponse response, IOplusCarrierIdentifierIndication indication);
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
