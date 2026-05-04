/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.ElUlCellInfoItem;

@VintfStability
parcelable ElUlCellInfo {
    ElUlCellInfoItem el_ul_pcell_info;
    byte el_lte_ul_scc_info_list_len;
    ElUlCellInfoItem[] el_lte_ul_scc_info_list;
    byte el_nr_ul_scc_info_list_len;
    ElUlCellInfoItem[] el_nr_ul_scc_info_list;
}
