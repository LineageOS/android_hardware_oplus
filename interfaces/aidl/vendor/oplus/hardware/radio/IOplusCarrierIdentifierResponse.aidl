/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.OplusRadioResponseInfo;

@VintfStability
interface IOplusCarrierIdentifierResponse {
    oneway void setCallbackResponse(int clientId);
    oneway void setCarrierIdentifierIndEnableResponse(in OplusRadioResponseInfo info);
}
