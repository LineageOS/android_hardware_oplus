/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.touch;

import vendor.oplus.hardware.touch.OplusTouchInfo;

@VintfStability
interface IOplusTouchEventCallback {
    void touchSendCommand(int clientFlag, in OplusTouchInfo info);
    oneway void touchSendCommandOneWay(int clientFlag, in OplusTouchInfo info);
}
