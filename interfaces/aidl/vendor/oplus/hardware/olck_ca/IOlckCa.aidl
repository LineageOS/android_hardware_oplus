/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.olck_ca;

@VintfStability
interface IOlckCa {
    int sendCommand(int cmdId, in @nullable byte[] request, out @nullable byte[] response);
}
