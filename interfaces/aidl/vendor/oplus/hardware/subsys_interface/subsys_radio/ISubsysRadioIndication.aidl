/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.ActionResult;
import vendor.oplus.hardware.subsys_interface.subsys_radio.CellInfos;
import vendor.oplus.hardware.subsys_interface.subsys_radio.CyberSenseHALCellInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.FenceStateInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.FenceRemovedInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.FenceMergedInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.FenceCreatedInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.ImsMessage;
import vendor.oplus.hardware.subsys_interface.subsys_radio.ImsRtpControlInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.ImsRtpRedunEventInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.ImsRtpState;
import vendor.oplus.hardware.subsys_interface.subsys_radio.MccChangeIndInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.NasAccessBarringStatusInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.NeighborCellInfos;
import vendor.oplus.hardware.subsys_interface.subsys_radio.Nr5gDrxType;
import vendor.oplus.hardware.subsys_interface.subsys_radio.NrcaInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.NwRateLimitingInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.RxBoostStateInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.RrcStatusIndInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.SceneMode;
import vendor.oplus.hardware.subsys_interface.subsys_radio.SimOverdueIndType;
import vendor.oplus.hardware.subsys_interface.subsys_radio.SimlockInfoType;
import vendor.oplus.hardware.subsys_interface.subsys_radio.SimlockStateType;
import vendor.oplus.hardware.subsys_interface.subsys_radio.SmartNetworkSelectInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.VodataEventInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.VonrBackoffInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.VonrRollbackInfo;

@VintfStability
oneway interface ISubsysRadioIndication {
    void radioCyberSenseCellInfoInd(int type, in CyberSenseHALCellInfo[] info);
    void radioCyberSenseEventInd(int type, int value);
    void radioDtmfStartInd(int type, byte tone_type);
    void radioDtmfStopInd(int type, byte tone_type);
    void radioImsMessageInd(int type, in ImsMessage imsMessage);
    void radioImsRtpStateInd(int type, in ImsRtpState rtpState);
    void radioLteCaInfoInd(int type, in int[] ca_info);
    void radioLteRegDomainInd(int type, int value);
    void radioNecInd(int type, in byte[] data);
    void radioNonddsPagingInd();
    void radioNr5gDrxInd(int type, in Nr5gDrxType info);
    void radioNr5gFreqTypeIndication(int type, int nr5gFreqType);
    void radioSimlockInfoChangedInd(int type, in SimlockInfoType info);
    void radioSimlockStateChangedInd(int type, in SimlockStateType state);
    void radioStateChanged(int type, int state);
    void radioSidoIssueInd(int type, in int[] params);
    void radioNrCapInd(int type, byte nr_cap);
    void radioNonddsNullPagingInd(int type, byte np_type, int rat, int errcode);
    void radioCssnfNumInd(int type, in byte[] data);
    void radioMipiOscFreqHopStateInd(int type, int value);
    void radioSimOverdueInd(int type, in SimOverdueIndType state);
    void radioNwRateLimitingInd(int type, in NwRateLimitingInfo limitInfo);
    void radioMdmTestInd(int type, int data);
    void radioMsimSubModeInd(int type, int msimSubMode, int dsdaTxMode);
    void radioQosPeriodReportInd(int type, in byte[] data);
    void radioQosEventReportInd(int type, in byte[] data);
    void radioLteCellInfoInd(int type, int pci, int arfcn, boolean sib24_available);
    void radioMccChangeInd(int type, in MccChangeIndInfo roamInfo);
    void radioHstModeInd(int type, int hstMode);
    void radioServingCellInfoInd(int type, in CellInfos info);
    void radioHyperUplinkStateInd(int type, int sceneState, int hyperUplinkState, int hyperUplinkType);
    void radioAtomDataInd(int type, in byte[] data);
    void radioLinkLatencyInfoInd(int type, int ulLatency);
    void radioImsRtpControlInd(int type, in ImsRtpControlInfo info);
    void radioNasAccessBarringStatusInd(int type, in NasAccessBarringStatusInfo nasAccessBarringStatusinfo);
    void radioNrcaInfoChangeInd(int type, in NrcaInfo nrcaInfo);
    void radioVonrBackoffInfoChangeInd(int type, in VonrBackoffInfo backoffInfo);
    void radioVonrRollbackInfoChangeInd(int type, in VonrRollbackInfo rollbackInfo);
    void radioSingleSimChangeInd(int type, byte status);
    void radioCarrierLockStatusChangeInd(int type, in byte[] data);
    void radioRegionLockStatusChangeInd(int type, in byte[] data);
    void radioAlertEventInd(int type, int alertId, int alertCause, in int[] alertParam);
    void radioVodataEventInd(int type, in VodataEventInfo vodataEventInfo);
    void radioAllCellInfoInd(int type, in CellInfos[] servingCellInfo, in NeighborCellInfos[] neighborCellInfos);
    void radioNetworkActionResultInd(int type, in ActionResult[] result);
    void radioSceneModeInd(int type, in SceneMode[] scenes);
    void radioSmartNetworkSelectInd(int type, in SmartNetworkSelectInfo smartNetworkSelectInfo);
    void radioImsRtpRedunEventInd(int type, in ImsRtpRedunEventInfo event);
    void rxBoostStateInd(int type, in RxBoostStateInfo state);
    void radioRrcStatusChangeInd(int type, in RrcStatusIndInfo rrcInfo);
    void radioFenceStateChangeInd(int type, in FenceStateInfo[] fenceStateInfos);
    void radioFenceRemovedInd(int type, in FenceRemovedInfo[] fenceRemovedInfos);
    void radioFenceMergedInd(int type, in FenceMergedInfo[] fenceMergedInfos);
    void radioFenceCreateInd(int type, in FenceCreatedInfo fenceCreatedInfo);
}
