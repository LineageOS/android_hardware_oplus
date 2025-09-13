/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.urcc;

import vendor.oplus.hardware.urcc.IUrccCallback;
import vendor.oplus.hardware.urcc.UrccRequestData;
import vendor.oplus.hardware.urcc.UrccRequestParcel;

@VintfStability
interface IUrcc {
    void urccInit();
    int urccResCtlRequest(in UrccRequestParcel mUrccRequestParcel);
    int urccResCtlRelease(int mhandle);
    UrccRequestData[] urccResStateRequest(in UrccRequestParcel mUrccRequestParcel);
    int urccResListeningRegister(in UrccRequestParcel mUrccRequestParcel, IUrccCallback urccCallback);
    int urccResListeningUnRegister(int mhandle);
    String urccPropertyGet(String name);
    int urccPropertySet(String name, String value);
    int urccThermalListeningRegister(in int[] types, IUrccCallback urccCallback);
    int urccThermalListeningUnRegister(int mhandle);
    int uahNotifyExt(int src, int type, in int[] args);
    int setRelatedSysInfo(int cmd, in byte[] info);
    int urccRuleCtl(int ruleId, int status, in UrccRequestData[] ruleData);
    void uahResCtlRequestBypass(in UrccRequestParcel mRequestParcel);
}
