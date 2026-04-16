/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.camera.aon;

import vendor.oplus.hardware.camera.aon.AONCallBackInfo;
import vendor.oplus.hardware.camera.aon.AONSensorStateInfo;

@VintfStability
interface IAONServiceCallBack {
    void onAONEvent(long clientHandle, in AONCallBackInfo aonCallBackInfo);
    void onAONSensorEvent(long clientHandle, in AONSensorStateInfo aonSensorStateInfo);
}
