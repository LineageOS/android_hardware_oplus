/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.cammidasservice;

import vendor.oplus.hardware.cammidasservice.CameraInfo;

@VintfStability
parcelable PowerFactor {
    String packageName;
    int numOfCameras;
    CameraInfo[] cameraInfoSet;
    double frameRate;
    double startTime;
    double duration;
    boolean isLaserOpen;
    double[2] flashTime;
    boolean isOISEnable;
    double[3] motorTime;
}
