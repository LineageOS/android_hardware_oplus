/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
///////////////////////////////////////////////////////////////////////////////
// THIS FILE IS IMMUTABLE. DO NOT EDIT IN ANY CASE.                          //
///////////////////////////////////////////////////////////////////////////////

// This file is a snapshot of an AIDL file. Do not edit it manually. There are
// two cases:
// 1). this is a frozen version file - do not edit this in any case.
// 2). this is a 'current' file. If you make a backwards compatible change to
//     the interface (from the latest frozen version), the build system will
//     prompt you to update this file with `m <name>-update-api`.
//
// You must not make a backward incompatible change to any AIDL file built
// with the aidl_interface module type with versions property set. The module
// type is used to build AIDL files in a way that they can be used across
// independently updatable components of the system. If a device is shipped
// with such a backward incompatible change, it has a high risk of breaking
// later when a module using the interface is updated, e.g., Mainline modules.

package vendor.oplus.hardware.subsys_interface.subsys_radio;
@VintfStability
interface ISubsysRadioIndication {
  oneway void radioCyberSenseCellInfoInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.CyberSenseHALCellInfo[] info);
  oneway void radioCyberSenseEventInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, int value);
  oneway void radioDtmfStartInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, byte tone_type);
  oneway void radioDtmfStopInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, byte tone_type);
  oneway void radioImsMessageInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.ImsMessage imsMessage);
  oneway void radioImsRtpStateInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.ImsRtpState rtpState);
  oneway void radioLteCaInfoInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in int[] ca_info);
  oneway void radioLteRegDomainInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, int value);
  oneway void radioNecInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in byte[] data);
  oneway void radioNr5gDrxInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.Nr5gDrxType info);
  oneway void radioNr5gFreqTypeIndication(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, int nr5gFreqType);
  oneway void radioSimlockInfoChangedInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.SimlockInfoType info);
  oneway void radioSimlockStateChangedInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.SimlockStateType state);
  oneway void radioStateChanged(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, vendor.oplus.hardware.subsys_interface.subsys.SubsysServiceState state);
  oneway void radioSidoIssueInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in int[] params);
  oneway void radioNrCapInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, byte nr_cap);
  oneway void radioNonddsNullPagingInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, byte np_type, int rat, int errcode);
  oneway void radioCssnfNumInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in byte[] data);
  oneway void radioMipiOscFreqHopStateInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, int value);
  oneway void radioSimOverdueInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.SimOverdueIndType state);
  oneway void radioNwRateLimitingInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.NwRateLimitingInfo limitInfo);
  oneway void radioMdmTestInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, int data);
  oneway void radioMsimSubModeInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, int msimSubMode, int dsdaTxMode);
  oneway void radioQosPeriodReportInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in byte[] data);
  oneway void radioQosEventReportInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in byte[] data);
  oneway void radioLteCellInfoInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, int pci, int arfcn, boolean sib24_available);
  oneway void radioMccChangeInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.MccChangeIndInfo roamInfo);
  oneway void radioHstModeInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, int hstMode);
  oneway void radioServingCellInfoInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.CellInfos info);
  oneway void radioHyperUplinkStateInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, int sceneState, int hyperUplinkState, int hyperUplinkType);
  oneway void radioAtomDataInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in byte[] data);
  oneway void radioLinkLatencyInfoInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, int ulLatency);
  oneway void radioImsRtpControlInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.ImsRtpControlInfo info);
  oneway void radioNasAccessBarringStatusInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.NasAccessBarringStatusInfo nasAccessBarringStatusinfo);
  oneway void radioNrcaInfoChangeInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.NrcaInfo nrcaInfo);
  oneway void radioVonrBackoffInfoChangeInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.VonrBackoffInfo backoffInfo);
  oneway void radioVonrRollbackInfoChangeInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.VonrRollbackInfo rollbackInfo);
  oneway void radioSingleSimChangeInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, byte status);
  oneway void radioCarrierLockStatusChangeInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in byte[] data);
  oneway void radioRegionLockStatusChangeInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in byte[] data);
  oneway void radioAlertEventInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, int alertId, int alertCause, in int[] alertParam);
  oneway void radioVodataEventInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.VodataEventInfo vodataEventInfo);
  oneway void radioAllCellInfoInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.CellInfos[] servingCellInfo, in vendor.oplus.hardware.subsys_interface.subsys_radio.NeighborCellInfos[] neighborCellInfos);
  oneway void radioNetworkActionResultInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.ActionResult[] result);
  oneway void radioSceneModeInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.SceneMode[] scenes);
  oneway void radioSmartNetworkSelectInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.SmartNetworkSelectInfo smartNetworkSelectInfo);
  oneway void radioImsRtpRedunEventInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.ImsRtpRedunEventInfo event);
  oneway void rxBoostStateInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.RxBoostStateInfo state);
  oneway void radioRrcStatusChangeInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.RrcStatusIndInfo rrcInfo);
  oneway void radioFenceStateChangeInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.FenceStateInfo[] fenceStateInfos);
  oneway void radioFenceRemovedInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.FenceRemovedInfo[] fenceRemovedInfos);
  oneway void radioFenceMergedInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.FenceMergedInfo[] fenceMergedInfos);
  oneway void radioFenceCreateInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.FenceCreatedInfo fenceCreatedInfo);
  oneway void radioSdfServiceStatusInd(int status);
  oneway void radioSdfAutoFilterInfoInd(int filterId, in vendor.oplus.hardware.subsys_interface.subsys_radio.SdfAddFilterInfo filterInfo, int status);
  oneway void radioGameLatencyPredictionResultInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.GameLatencyPredictionResultInfo gameLatencyPredictionResultInfo);
  oneway void radioUnitTestResultInd(vendor.oplus.hardware.subsys_interface.subsys.SubsysIndicationType type, in vendor.oplus.hardware.subsys_interface.subsys_radio.UnitTestResults results);
}
