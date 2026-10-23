/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable AdaptiveHandoverThreshold {
    int good_cell_enable = 0;
    int ncell_select_enable = 0;
    int good_cell_num = 0;
    int rsrp = 0;
    int sinr = 0;
    int score = 0;
}
