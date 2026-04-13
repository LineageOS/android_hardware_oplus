/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys;

import vendor.oplus.hardware.subsys_interface.subsys.SubsysResponseInfo;
import vendor.oplus.hardware.subsys_interface.subsys.ImsRtpRedunSignalMsgRspInfo;

@VintfStability
interface IImsOrtcResponse {
    void setCallInfoResponse(in SubsysResponseInfo info);
    void setOrtcRoomInfoResponse(in SubsysResponseInfo info);
    void sendImsRtpDataResponse(in SubsysResponseInfo info);
    void setImsRtpRemotePubIpAddrResponse(in SubsysResponseInfo info);
    void sendImsRtpRedunSignalMsgResponse(in SubsysResponseInfo info, in ImsRtpRedunSignalMsgRspInfo msgRsp);
}
