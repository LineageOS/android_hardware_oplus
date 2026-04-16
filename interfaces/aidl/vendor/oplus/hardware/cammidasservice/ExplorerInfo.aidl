/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.cammidasservice;

import vendor.oplus.hardware.cammidasservice.ExplorerExceptionInfo;
import vendor.oplus.hardware.cammidasservice.ExplorerModeInfo;
import vendor.oplus.hardware.cammidasservice.ExplorerTempInfo;

@VintfStability
parcelable ExplorerInfo {
    int operationMode;
    double startTime;
    double endTime;
    double[8] sensorDuration;
    ExplorerExceptionInfo exceptionInfo;
    ExplorerModeInfo modeInfo;
    ExplorerTempInfo tempInfo;
}
