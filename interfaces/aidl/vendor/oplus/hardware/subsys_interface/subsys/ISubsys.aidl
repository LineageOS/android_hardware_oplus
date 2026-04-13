/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys;

import vendor.oplus.hardware.subsys_interface.subsys.CallInfo;
import vendor.oplus.hardware.subsys_interface.subsys.IccReqInfo;
import vendor.oplus.hardware.subsys_interface.subsys.IccMetaReqInfo;
import vendor.oplus.hardware.subsys_interface.subsys.ImsRtpDataInfo;
import vendor.oplus.hardware.subsys_interface.subsys.ImsRtpPubIpAddrInfo;
import vendor.oplus.hardware.subsys_interface.subsys.ImsRtpRedunSignalMsg;
import vendor.oplus.hardware.subsys_interface.subsys.OrtcRoomInfo;
import vendor.oplus.hardware.subsys_interface.subsys.ISubsysResponse;
import vendor.oplus.hardware.subsys_interface.subsys.ISubsysIndication;
import vendor.oplus.hardware.subsys_interface.subsys.ISubsysIccMetaCallback;
import vendor.oplus.hardware.subsys_interface.subsys.IImsOrtcResponse;
import vendor.oplus.hardware.subsys_interface.subsys.IImsOrtcIndication;

@VintfStability
interface ISubsys {
    void getSwtpStatus(int serial);
    void setCallback(ISubsysResponse responseCallback, ISubsysIndication indicationCallback);
    void getLcdInfo(int serial, int displayMappingIdx);
    void setMipiFreqClk(int serial, int displayMappingIdx, long mipiClk);
    void setOscFreqClk(int serial, int displayMappingIdx, int oscClk);
    void getNvramData(int serial, int index, int size);
    void setImsOrtcCallback(IImsOrtcResponse responseCallback, IImsOrtcIndication indicationCallback);
    void setCallInfo(int serial, in CallInfo info);
    void setOrtcRoomInfo(int serial, in OrtcRoomInfo info);
    void sendImsRtpData(int serial, in ImsRtpDataInfo info);
    void setImsRtpRemotePubIpAddr(int serial, in ImsRtpPubIpAddrInfo info);
    void setIccTuningState(int serial, int state, int freq, int mode, int ant);
    void setIccWifiAntState(int serial, int channel, int state);
    void setIccSatelliteAntState(int serial, int satType, int on, int ant);
    void getIccState(int serial);
    void killProcess(int serial, int pid, String processName);
    void sendIccCmd(int serial, in IccReqInfo info);
    void sendIccMetaCmd(in IccMetaReqInfo reqInfo, ISubsysIccMetaCallback cb);
    void sendImsRtpRedunSignalMsg(int serial, in ImsRtpRedunSignalMsg msg);
}
