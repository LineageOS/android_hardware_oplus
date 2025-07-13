/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.OPLUS_RIL_Rx_Chain_info;
import vendor.oplus.hardware.radio.OPLUS_RIL_Tx_info;

@VintfStability
parcelable OPLUS_RIL_Tx_Rx_info {
    byte rx_chain_0_valid;
    OPLUS_RIL_Rx_Chain_info rx_chain_0;
    byte rx_chain_1_valid;
    OPLUS_RIL_Rx_Chain_info rx_chain_1;
    byte rx_chain_2_valid;
    OPLUS_RIL_Rx_Chain_info rx_chain_2;
    byte rx_chain_3_valid;
    OPLUS_RIL_Rx_Chain_info rx_chain_3;
    byte tx_valid;
    OPLUS_RIL_Tx_info tx;
}
