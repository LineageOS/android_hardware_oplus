/*
 * Copyright (C) 2024-2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.touch;

import vendor.oplus.hardware.touch.OplusTouchInfo;

@VintfStability
interface IOplusTouch {
    int initialize();
    int isTouchNodeSupport(int deviceId, int nodeFlag);
    String touchReadNodeFile(int deviceId, int nodeFlag);
    int touchWriteNodeFile(int deviceId, int nodeFlag, String info);
    int touchWriteBtInfo(int deviceId, int nodeFlag, String info);
    void touchWriteNodeFileOneWay(int deviceId, int nodeFlag, String info);
    int touchNotifyClient(int clientFlag, in OplusTouchInfo info);
}
