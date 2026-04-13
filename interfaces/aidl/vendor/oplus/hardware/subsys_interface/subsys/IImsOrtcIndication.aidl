/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys;

import vendor.oplus.hardware.subsys_interface.subsys.ImsRtpPubIpAddrInfo;
import vendor.oplus.hardware.subsys_interface.subsys.ImsRtpDataInfo;
import vendor.oplus.hardware.subsys_interface.subsys.OrtcStatus;
import vendor.oplus.hardware.subsys_interface.subsys.OrtcConnectInfo;
import vendor.oplus.hardware.subsys_interface.subsys.ImsRtpRedunSignalMsg;
import vendor.oplus.hardware.subsys_interface.subsys.ImsRtpRedunDataLinkStateInfo;

@VintfStability
interface IImsOrtcIndication {
    void LocalPubIpAddrChanged(int type, in ImsRtpPubIpAddrInfo info);
    void recvImsRtpData(int type, in ImsRtpDataInfo info);
    void notifyOrtcStatus(int type, in OrtcStatus status);
    void notifyConnectInfo(int type, in OrtcConnectInfo info);
    void recvImsRtpRedunSignalMsgInd(int type, in ImsRtpRedunSignalMsg msg);
    void notifyImsRtpRedunDataLinkStateInd(int type, in ImsRtpRedunDataLinkStateInfo info);
}
