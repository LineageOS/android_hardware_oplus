/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.ElDlCellInfoItem;

@VintfStability
parcelable ElDlCellInfo {
    ElDlCellInfoItem el_dl_pcell_info;
    byte el_lte_dl_scc_info_list_len;
    ElDlCellInfoItem[] el_lte_dl_scc_info_list;
    byte el_nr_dl_scc_info_list_len;
    ElDlCellInfoItem[] el_nr_dl_scc_info_list;
}
