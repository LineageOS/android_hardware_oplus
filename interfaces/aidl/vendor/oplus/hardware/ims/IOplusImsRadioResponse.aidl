/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.ims;

import vendor.oplus.hardware.ims.OplusImsRadioResponseInfo;

@VintfStability
interface IOplusImsRadioResponse {
    oneway void queryVopsStatusResponse(in OplusImsRadioResponseInfo info, int result, boolean isVopsEnabled);
    oneway void sendOemCommandResponse(in OplusImsRadioResponseInfo info, int result);
}
