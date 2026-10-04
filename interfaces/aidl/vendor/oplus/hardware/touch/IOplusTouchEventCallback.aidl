/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.touch;

import vendor.oplus.hardware.touch.OplusTouchEventInfo;

@VintfStability
interface IOplusTouchEventCallback {
    void touchSendCommand(int clientFlag, in OplusTouchEventInfo info);
    oneway void touchSendCommandOneWay(int clientFlag, in OplusTouchEventInfo info);
}
