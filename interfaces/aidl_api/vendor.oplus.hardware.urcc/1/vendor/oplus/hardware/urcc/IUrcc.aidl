/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
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

package vendor.oplus.hardware.urcc;
@VintfStability
interface IUrcc {
  void urccInit();
  int urccResCtlRequest(in vendor.oplus.hardware.urcc.UrccRequestParcel mUrccRequestParcel);
  int urccResCtlRelease(int mhandle);
  vendor.oplus.hardware.urcc.UrccRequestData[] urccResStateRequest(in vendor.oplus.hardware.urcc.UrccRequestParcel mUrccRequestParcel);
  int urccResListeningRegister(in vendor.oplus.hardware.urcc.UrccRequestParcel mUrccRequestParcel, vendor.oplus.hardware.urcc.IUrccCallback urccCallback);
  int urccResListeningUnRegister(int mhandle);
  String urccPropertyGet(String name);
  int urccPropertySet(String name, String value);
  int urccThermalListeningRegister(in int[] types, vendor.oplus.hardware.urcc.IUrccCallback urccCallback);
  int urccThermalListeningUnRegister(int mhandle);
  int uahNotifyExt(int src, int type, in int[] args);
  int setRelatedSysInfo(int cmd, in byte[] info);
  int urccRuleCtl(int ruleId, int status, in vendor.oplus.hardware.urcc.UrccRequestData[] ruleData);
  void uahResCtlRequestBypass(in vendor.oplus.hardware.urcc.UrccRequestParcel mRequestParcel);
}
