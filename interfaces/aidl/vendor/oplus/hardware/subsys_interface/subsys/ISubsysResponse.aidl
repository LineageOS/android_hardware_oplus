/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys;

import vendor.oplus.hardware.subsys_interface.subsys.SubsysResponseInfo;
import vendor.oplus.hardware.subsys_interface.subsys.IccStateInfo;
import vendor.oplus.hardware.subsys_interface.subsys.IccRespInfo;

@VintfStability
interface ISubsysResponse {
    void getSwtpStatusResponse(in SubsysResponseInfo info, byte result);
    void getLcdInfoResponse(in SubsysResponseInfo info, String lcdInfo);
    void setMipiFreqClkResponse(in SubsysResponseInfo info);
    void setOscFreqClkResponse(in SubsysResponseInfo info);
    void getNvramDataResponse(in SubsysResponseInfo info, String bufInfo);
    void setIccTuningStateResponse(in SubsysResponseInfo info);
    void setIccWifiAntStateResponse(in SubsysResponseInfo info);
    void setIccSatelliteAntStateResponse(in SubsysResponseInfo info);
    void getIccStateResponse(in SubsysResponseInfo info, in IccStateInfo state);
    void killProcessResponse(in SubsysResponseInfo info);
    void sendIccCmdResponse(in SubsysResponseInfo info, in IccRespInfo respInfo);
}
