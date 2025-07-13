/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.OplusRadioResponseInfo;

@VintfStability
interface IOplusCarrierIdentifierResponse {
    void setCallbackResponse(int result);
    void setCarrierIdentifierIndEnableResponse(in OplusRadioResponseInfo info);
}
