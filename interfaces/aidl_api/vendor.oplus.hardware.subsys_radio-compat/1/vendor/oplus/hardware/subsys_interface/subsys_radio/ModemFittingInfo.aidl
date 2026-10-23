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
parcelable ModemFittingInfo {
  int startTime = 0;
  int endTime = 0;
  int version = 0;
  int interval = 0;
  int[] modemPwr;
  int[] rfPwr;
  int[] transPwr;
  int nSrchT = 0;
  int nPlmnSrchT = 0;
  int nPwrScanT = 0;
  int nOosSrchT = 0;
  int nPlmnlistT = 0;
  int nSnifferT = 0;
  int nBblSrchT = 0;
  int nConRelT = 0;
  int nRelRirSrchT = 0;
  int nReestSrchT = 0;
  int nOthSrchT = 0;
  int nSrvMeasC = 0;
  int nNeibIntraMeasC = 0;
  int nNeibInterMeasC = 0;
  int nNeibIratMeasC = 0;
  int nReselIntraT = 0;
  int nReselInterT = 0;
  int nHoIntraT = 0;
  int nHoInterT = 0;
  int nRedirIntraT = 0;
  int nRedirInterT = 0;
  int nReselIntraC = 0;
  int nReselInterC = 0;
  int nHoIntraC = 0;
  int nHoInterC = 0;
  int nRedirIntraC = 0;
  int nRedirInterC = 0;
  int nTxT = 0;
  int nTxL1T = 0;
  int nTxL2T = 0;
  int nTxL3T = 0;
  int nTxL4T = 0;
  int nTxL5T = 0;
  int nTxL6T = 0;
  int nTxL7T = 0;
  int nTxL8T = 0;
  int nTxL9T = 0;
  int nTxL10T = 0;
  int nTxL11T = 0;
  int nTxL12T = 0;
  int nTxL13T = 0;
  int nTxL14T = 0;
  int nTxL15T = 0;
  int nTxL16T = 0;
  int nTxL17T = 0;
  int nTxL18T = 0;
  int nTxL19T = 0;
  int nTxL20T = 0;
  int nTxL21T = 0;
  int nTxL22T = 0;
  int nTxL23T = 0;
  int nTxL24T = 0;
  int nTxL25T = 0;
  int nTxL26T = 0;
  int nTxL27T = 0;
  int nTxL28T = 0;
  int nTxL29T = 0;
  int nTxL30T = 0;
  int n1TxT = 0;
  int n2TxT = 0;
  int nUl1layerT = 0;
  int nUl2layerT = 0;
  int nUl3layerT = 0;
  int nUl4layerT = 0;
  int n1RxT = 0;
  int n2RxT = 0;
  int n3RxT = 0;
  int n4RxT = 0;
  int nDl1layerT = 0;
  int nDl2layerT = 0;
  int nDl3layerT = 0;
  int nDl4layerT = 0;
  int nRrcconT = 0;
  int nRrcIdleT = 0;
  int nCdrxInactT = 0;
  int nRachC = 0;
  int nTauC = 0;
  int nMrC = 0;
  int nReestC = 0;
  int nRreestT = 0;
  int lSrchT = 0;
  int lPlmnSrchT = 0;
  int lOosSrchT = 0;
  int lBgSrchT = 0;
  int lSrvMeasC = 0;
  int lNeibIntraMeasC = 0;
  int lNeibInterMeasC = 0;
  int lNeibIratMeasC = 0;
  int lReselIntraT = 0;
  int lReselInterT = 0;
  int lHoIntraT = 0;
  int lHoInterT = 0;
  int lRedirIntraT = 0;
  int lRedirInterT = 0;
  int lReselIntraC = 0;
  int lReselInterC = 0;
  int lHoIntraC = 0;
  int lHoInterC = 0;
  int lRedirIntraC = 0;
  int lRedirInterC = 0;
  int lTxT = 0;
  int lTxL1T = 0;
  int lTxL2T = 0;
  int lTxL3T = 0;
  int lTxL4T = 0;
  int lTxL5T = 0;
  int lTxL6T = 0;
  int lTxL7T = 0;
  int lTxL8T = 0;
  int lTxL9T = 0;
  int lTxL10T = 0;
  int lTxL11T = 0;
  int lTxL12T = 0;
  int lTxL13T = 0;
  int lTxL14T = 0;
  int lTxL15T = 0;
  int lTxL16T = 0;
  int lTxL17T = 0;
  int lTxL18T = 0;
  int lTxL19T = 0;
  int lTxL20T = 0;
  int lTxL21T = 0;
  int lTxL22T = 0;
  int lTxL23T = 0;
  int lTxL24T = 0;
  int lTxL25T = 0;
  int lTxL26T = 0;
  int lTxL27T = 0;
  int lTxL28T = 0;
  int lTxL29T = 0;
  int lTxL30T = 0;
  int l1TxT = 0;
  int l2TxT = 0;
  int lUl1layerT = 0;
  int lUl2layerT = 0;
  int lUl3layerT = 0;
  int lUl4layerT = 0;
  int l1RxT = 0;
  int l2RxT = 0;
  int l3RxT = 0;
  int l4RxT = 0;
  int lDl1layerT = 0;
  int lDl2layerT = 0;
  int lDl3layerT = 0;
  int lDl4layerT = 0;
  int lRrcconnT = 0;
  int lRrcIdleT = 0;
  int lCdrxInactT = 0;
  int lRachC = 0;
  int lTauC = 0;
  int lMrC = 0;
  int lReestC = 0;
}
