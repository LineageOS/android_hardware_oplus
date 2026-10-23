/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.RxChainInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.TxInfo;

@VintfStability
parcelable TxRxInfo {
    byte rxChain0Valid = 0;
    RxChainInfo rxChain0;
    byte rxChain1Valid = 0;
    RxChainInfo rxChain1;
    byte rxChain2Valid = 0;
    RxChainInfo rxChain2;
    byte rxChain3Valid = 0;
    RxChainInfo rxChain3;
    byte txValid = 0;
    TxInfo tx;
}
