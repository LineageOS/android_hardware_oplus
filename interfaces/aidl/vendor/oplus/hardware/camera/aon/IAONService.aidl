/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.camera.aon;

import vendor.oplus.hardware.camera.aon.AONSensorInfo;
import vendor.oplus.hardware.camera.aon.IAONServiceCallBack;
import vendor.oplus.hardware.camera.aon.SensorConfigInfo;

@VintfStability
interface IAONService {
    long registerClient(IAONServiceCallBack callback, int serviceType);
    AONSensorInfo[] getAonSensorInfoList();
    void subscribeSensor(long clientHandle, in SensorConfigInfo sensorConfigInfo);
    void startService(long clientHandle, int serviceType);
    void stopService(long clientHandle, int serviceType);
    void unsubscribeSensor(long clientHandle);
    void unregisterClient(long clientHandle);
}
