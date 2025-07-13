/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.CarrierIdentifier;

@VintfStability
interface IOplusCarrierIdentifierIndication {
    void carrierIdentifierIndication(in CarrierIdentifier carrierIdentifier);
}
