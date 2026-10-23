/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable IdcOffsetConfig {
    int valid = 0;
    int start_freq = 0;
    int end_freq = 0;
    int offset = 0;
}
