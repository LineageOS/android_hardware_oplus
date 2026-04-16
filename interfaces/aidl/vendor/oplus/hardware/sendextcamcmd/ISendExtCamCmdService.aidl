/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.sendextcamcmd;

import vendor.oplus.hardware.sendextcamcmd.ExtCamCmd;

@VintfStability
interface ISendExtCamCmdService {
    int sendOplusExtCamCmd(in ExtCamCmd cmd);
    String readOplusExtCamCmd(in ExtCamCmd cmd);
}
