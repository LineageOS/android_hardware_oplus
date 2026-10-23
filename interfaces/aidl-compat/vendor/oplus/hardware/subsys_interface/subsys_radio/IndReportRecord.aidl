/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable IndReportRecord {
    int ind_msg_id = 0;
    int wakeup_times = 0;
    int discard_times = 0;
    int aligned_times = 0;
}
