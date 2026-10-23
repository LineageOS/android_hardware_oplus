/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.ims;

@VintfStability
interface IOplusImsRadioResponse {
    oneway void queryVopsStatusResponse(int serial, int result, boolean isVopsEnabled);
    oneway void sendOemCommandResponse(int serial, int result);
}
