/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.urcc;

import vendor.oplus.hardware.urcc.IUrccCallback;
import vendor.oplus.hardware.urcc.UrccRequestData;

@VintfStability
parcelable UrccRequestParcel {
    String identity;
    String[] mParams;
    UrccRequestData[] mUrccRequestData;
    byte[] otherData;
    String pkgName;
    IUrccCallback urccCallback;
    int type;
    boolean screenoff_support;
    int prio;
    int callingPID;
    int callingUID;
    long duration;
    int handle;
}
