/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
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
    void setModemErrorFatal(int serial, int reasonType);
    void setVoNrEnabled(int serial, boolean enable);
    void getVoNrEnabled(int serial);
    void setEccList(int serial, in EccEntryInfo[] eccEntryInfos);
    void setHsrModeForListen(int serial, int value);
    void setLogIdForListen(int serial, int operateType, int logType, int logId, int subId);
    void setSlowStartConfig(int serial, int percent);
    void setCtVolteMode(int serial, int volteMode);
    void simlockReq(int serial, in byte[] data, int datalen);
    void updateRegionlockBlob(int serial, in byte[] data, int datalen);
    void updateRegionlockStatus(int serial, byte regionlock_operator, byte operation, byte data);
    void getRegionlockStatus(int serial);
    void setNwCongestionCfg(int serial, in byte[] data, int datalen);
    void setModemEsimStatus(int serial, boolean status);
    void setCallback(in IOplusRadioResponse responseCallback, in IOplusRadioIndication indicationCallback);
    void setCarrierIdentifierIndEnable(int serial, boolean enable);
    void setCarrierIdentifierCallback(in IOplusCarrierIdentifierResponse respCallback, in IOplusCarrierIdentifierIndication indCallback);
    void getSimlockOperatorId(int serial);
    void getSimlockFeestate(int serial);
    void setSimlockFeestate(int serial, byte feestate);
    void getSimlockUnlockState(int serial);
    void setSimlockFactoryResetTime(int serial, in byte[] data, int datalen);
    void getSimlockFactoryResetTime(int serial);
    void setSimlockActivateTime(int serial, in byte[] data, int datalen);
    void getSimlockActivateTime(int serial);
    void getSimlockFeature(int serial);
    void getSimlockLockmark(int serial);
    void getSimlockDeviceLockState(int serial);
    void getSimlockDeviceLockinfo(int serial);
    void getSimlockVersionInfo(int serial);
    void getSimlockMaxRetry(int serial);
    void getSimlockCurrentRetry(int serial);
    void setPsDetachAttachAction(int serial, int action);
    void unlockRegionlock(int serial, in byte[] data, byte mode);
    void updateRegionlockKey(int serial, in byte[] data, in byte[] key);
    void getRegionlockSettingData(int serial);
    void setRegionlockSettingData(int serial, in byte[] data);
    void setSimlockOfflineLock(int serial, boolean enabled, int timeout, in int[] retryCount, long timestamp);
    void getSimlockOfflineLock(int serial);
}
