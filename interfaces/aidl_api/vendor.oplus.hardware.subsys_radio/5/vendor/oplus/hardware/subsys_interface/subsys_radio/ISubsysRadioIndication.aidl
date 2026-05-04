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
  oneway void radioCyberSenseCellInfoInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.CyberSenseHALCellInfo[] info);
  oneway void radioCyberSenseEventInd(int type, int value);
  oneway void radioDtmfStartInd(int type, byte tone_type);
  oneway void radioDtmfStopInd(int type, byte tone_type);
  oneway void radioImsMessageInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.ImsMessage imsMessage);
  oneway void radioImsRtpStateInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.ImsRtpState rtpState);
  oneway void radioLteCaInfoInd(int type, in int[] ca_info);
  oneway void radioLteRegDomainInd(int type, int value);
  oneway void radioNecInd(int type, in byte[] data);
  oneway void radioNonddsPagingInd();
  oneway void radioNr5gDrxInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.Nr5gDrxType info);
  oneway void radioNr5gFreqTypeIndication(int type, int nr5gFreqType);
  oneway void radioSimlockInfoChangedInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.SimlockInfoType info);
  oneway void radioSimlockStateChangedInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.SimlockStateType state);
  oneway void radioStateChanged(int type, int state);
  oneway void radioSidoIssueInd(int type, in int[] params);
  oneway void radioNrCapInd(int type, byte nr_cap);
  oneway void radioNonddsNullPagingInd(int type, byte np_type, int rat, int errcode);
  oneway void radioCssnfNumInd(int type, in byte[] data);
  oneway void radioMipiOscFreqHopStateInd(int type, int value);
  oneway void radioSimOverdueInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.SimOverdueIndType state);
  oneway void radioNwRateLimitingInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.NwRateLimitingInfo limitInfo);
  oneway void radioMdmTestInd(int type, int data);
  oneway void radioMsimSubModeInd(int type, int msimSubMode, int dsdaTxMode);
  oneway void radioQosPeriodReportInd(int type, in byte[] data);
  oneway void radioQosEventReportInd(int type, in byte[] data);
  oneway void radioLteCellInfoInd(int type, int pci, int arfcn, boolean sib24_available);
  oneway void radioMccChangeInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.MccChangeIndInfo roamInfo);
  oneway void radioHstModeInd(int type, int hstMode);
  oneway void radioServingCellInfoInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.CellInfos info);
  oneway void radioHyperUplinkStateInd(int type, int sceneState, int hyperUplinkState, int hyperUplinkType);
  oneway void radioAtomDataInd(int type, in byte[] data);
  oneway void radioLinkLatencyInfoInd(int type, int ulLatency);
  oneway void radioImsRtpControlInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.ImsRtpControlInfo info);
  oneway void radioNasAccessBarringStatusInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.NasAccessBarringStatusInfo nasAccessBarringStatusinfo);
  oneway void radioNrcaInfoChangeInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.NrcaInfo nrcaInfo);
  oneway void radioVonrBackoffInfoChangeInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.VonrBackoffInfo backoffInfo);
  oneway void radioVonrRollbackInfoChangeInd(int type, in vendor.oplus.hardware.subsys_interface.subsys_radio.VonrRollbackInfo rollbackInfo);
  oneway void radioSingleSimChangeInd(int type, byte status);
  oneway void radioCarrierLockStatusChangeInd(int type, in byte[] data);
  oneway void radioRegionLockStatusChangeInd(int type, in byte[] data);
  oneway void radioAlertEventInd(int type, int alertId, int alertCause, in int[] alertParam);
}
