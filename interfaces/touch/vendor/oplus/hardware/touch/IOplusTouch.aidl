/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.touch;

@VintfStability
interface IOplusTouch {
    int initialize();
    int isTouchNodeSupport(int deviceId, int nodeFlag);
    String touchReadNodeFile(int deviceId, int nodeFlag);
    int touchWriteNodeFile(int deviceId, int nodeFlag, String info);
}
