/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable ElUlCellInfoItem {
    byte techType;
    int bandNumber = 0;
    int arfcn = 0;
    int bandWidth = 0;
    byte isPrimary;
}
