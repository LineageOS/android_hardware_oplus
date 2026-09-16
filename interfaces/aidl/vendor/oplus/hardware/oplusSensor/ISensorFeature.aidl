/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.oplusSensor;

@VintfStability
interface ISensorFeature {
    boolean calibrateSensor(int sensor_type, int para);
    String getSensorCalibrationData(int sensor_type);
    String getSensorConfig();
    float getSensorRawData(int sensor_type);
    int saveCalibrationData(int sensor_type, int type, in byte[] cali_data);
    boolean sensorRawDataEnableDisable(int sensor_type, int para);
    String sensorReadNodeFile(String node_name, int length);
    int sensorWriteNodeFile(String node_name, String info);
    int setPhoneMode(byte type, byte on);
}
