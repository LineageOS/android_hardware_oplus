/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.camera.aon;

@VintfStability
union ResultData {
    byte byteData;
    byte[] byteArrayData;
    int intData;
    int[] intArrayData;
    String stringData;
    float floatData;
    float[] floatArrayData;
    double doubleData;
    double[] doubleArrayData;
    boolean boolData;
}
