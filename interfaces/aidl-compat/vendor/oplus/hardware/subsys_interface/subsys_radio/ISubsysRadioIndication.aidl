/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType;
import vendor.oplus.hardware.subsys_interface.subsys.SubsysServiceState;
import vendor.oplus.hardware.subsys_interface.subsys_radio.ActionResult;
import vendor.oplus.hardware.subsys_interface.subsys_radio.CellInfos;
import vendor.oplus.hardware.subsys_interface.subsys_radio.CyberSenseHALCellInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.FenceCreatedInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.FenceMergedInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.FenceRemovedInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.FenceStateInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.GameLatencyPredictionResultInfo;
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
import vendor.oplus.hardware.subsys_interface.subsys_radio.RrcStatusIndInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.RxBoostStateInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.SceneMode;
import vendor.oplus.hardware.subsys_interface.subsys_radio.SdfAddFilterInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.SimOverdueIndType;
import vendor.oplus.hardware.subsys_interface.subsys_radio.SimlockInfoType;
import vendor.oplus.hardware.subsys_interface.subsys_radio.SimlockStateType;
import vendor.oplus.hardware.subsys_interface.subsys_radio.SmartNetworkSelectInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.UnitTestResults;
import vendor.oplus.hardware.subsys_interface.subsys_radio.VodataEventInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.VonrBackoffInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.VonrRollbackInfo;

@VintfStability
interface ISubsysRadioIndication {
    oneway void radioCyberSenseCellInfoInd(SubsysIndicationType type, in CyberSenseHALCellInfo[] info);
    oneway void radioCyberSenseEventInd(SubsysIndicationType type, int value);
    oneway void radioDtmfStartInd(SubsysIndicationType type, byte tone_type);
    oneway void radioDtmfStopInd(SubsysIndicationType type, byte tone_type);
    oneway void radioImsMessageInd(SubsysIndicationType type, in ImsMessage imsMessage);
    oneway void radioImsRtpStateInd(SubsysIndicationType type, in ImsRtpState rtpState);
    oneway void radioLteCaInfoInd(SubsysIndicationType type, in int[] ca_info);
    oneway void radioLteRegDomainInd(SubsysIndicationType type, int value);
    oneway void radioNecInd(SubsysIndicationType type, in byte[] data);
    oneway void radioNr5gDrxInd(SubsysIndicationType type, in Nr5gDrxType info);
    oneway void radioNr5gFreqTypeIndication(SubsysIndicationType type, int nr5gFreqType);
    oneway void radioSimlockInfoChangedInd(SubsysIndicationType type, in SimlockInfoType info);
    oneway void radioSimlockStateChangedInd(SubsysIndicationType type, in SimlockStateType state);
    oneway void radioStateChanged(SubsysIndicationType type, SubsysServiceState state);
    oneway void radioSidoIssueInd(SubsysIndicationType type, in int[] params);
    oneway void radioNrCapInd(SubsysIndicationType type, byte nr_cap);
    oneway void radioNonddsNullPagingInd(SubsysIndicationType type, byte np_type, int rat, int errcode);
    oneway void radioCssnfNumInd(SubsysIndicationType type, in byte[] data);
    oneway void radioMipiOscFreqHopStateInd(SubsysIndicationType type, int value);
    oneway void radioSimOverdueInd(SubsysIndicationType type, in SimOverdueIndType state);
    oneway void radioNwRateLimitingInd(SubsysIndicationType type, in NwRateLimitingInfo limitInfo);
    oneway void radioMdmTestInd(SubsysIndicationType type, int data);
    oneway void radioMsimSubModeInd(SubsysIndicationType type, int msimSubMode, int dsdaTxMode);
    oneway void radioQosPeriodReportInd(SubsysIndicationType type, in byte[] data);
    oneway void radioQosEventReportInd(SubsysIndicationType type, in byte[] data);
    oneway void radioLteCellInfoInd(SubsysIndicationType type, int pci, int arfcn, boolean sib24_available);
    oneway void radioMccChangeInd(SubsysIndicationType type, in MccChangeIndInfo roamInfo);
    oneway void radioHstModeInd(SubsysIndicationType type, int hstMode);
    oneway void radioServingCellInfoInd(SubsysIndicationType type, in CellInfos info);
    oneway void radioHyperUplinkStateInd(SubsysIndicationType type, int sceneState, int hyperUplinkState, int hyperUplinkType);
    oneway void radioAtomDataInd(SubsysIndicationType type, in byte[] data);
    oneway void radioLinkLatencyInfoInd(SubsysIndicationType type, int ulLatency);
    oneway void radioImsRtpControlInd(SubsysIndicationType type, in ImsRtpControlInfo info);
    oneway void radioNasAccessBarringStatusInd(SubsysIndicationType type, in NasAccessBarringStatusInfo nasAccessBarringStatusinfo);
    oneway void radioNrcaInfoChangeInd(SubsysIndicationType type, in NrcaInfo nrcaInfo);
    oneway void radioVonrBackoffInfoChangeInd(SubsysIndicationType type, in VonrBackoffInfo backoffInfo);
    oneway void radioVonrRollbackInfoChangeInd(SubsysIndicationType type, in VonrRollbackInfo rollbackInfo);
    oneway void radioSingleSimChangeInd(SubsysIndicationType type, byte status);
    oneway void radioCarrierLockStatusChangeInd(SubsysIndicationType type, in byte[] data);
    oneway void radioRegionLockStatusChangeInd(SubsysIndicationType type, in byte[] data);
    oneway void radioAlertEventInd(SubsysIndicationType type, int alertId, int alertCause, in int[] alertParam);
    oneway void radioVodataEventInd(SubsysIndicationType type, in VodataEventInfo vodataEventInfo);
    oneway void radioAllCellInfoInd(SubsysIndicationType type, in CellInfos[] servingCellInfo, in NeighborCellInfos[] neighborCellInfos);
    oneway void radioNetworkActionResultInd(SubsysIndicationType type, in ActionResult[] result);
    oneway void radioSceneModeInd(SubsysIndicationType type, in SceneMode[] scenes);
    oneway void radioSmartNetworkSelectInd(SubsysIndicationType type, in SmartNetworkSelectInfo smartNetworkSelectInfo);
    oneway void radioImsRtpRedunEventInd(SubsysIndicationType type, in ImsRtpRedunEventInfo event);
    oneway void rxBoostStateInd(SubsysIndicationType type, in RxBoostStateInfo state);
    oneway void radioRrcStatusChangeInd(SubsysIndicationType type, in RrcStatusIndInfo rrcInfo);
    oneway void radioFenceStateChangeInd(SubsysIndicationType type, in FenceStateInfo[] fenceStateInfos);
    oneway void radioFenceRemovedInd(SubsysIndicationType type, in FenceRemovedInfo[] fenceRemovedInfos);
    oneway void radioFenceMergedInd(SubsysIndicationType type, in FenceMergedInfo[] fenceMergedInfos);
    oneway void radioFenceCreateInd(SubsysIndicationType type, in FenceCreatedInfo fenceCreatedInfo);
    oneway void radioSdfServiceStatusInd(int status);
    oneway void radioSdfAutoFilterInfoInd(int filterId, in SdfAddFilterInfo filterInfo, int status);
    oneway void radioGameLatencyPredictionResultInd(SubsysIndicationType type, in GameLatencyPredictionResultInfo gameLatencyPredictionResultInfo);
    oneway void radioUnitTestResultInd(SubsysIndicationType type, in UnitTestResults results);
}
