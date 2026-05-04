/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.RxChainInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.TxInfo;

@VintfStability
parcelable TxRxInfo {
    byte rxChain0Valid;
    RxChainInfo rxChain0;
    byte rxChain1Valid;
    RxChainInfo rxChain1;
    byte rxChain2Valid;
    RxChainInfo rxChain2;
    byte rxChain3Valid;
    RxChainInfo rxChain3;
    byte txValid;
    TxInfo tx;
}
