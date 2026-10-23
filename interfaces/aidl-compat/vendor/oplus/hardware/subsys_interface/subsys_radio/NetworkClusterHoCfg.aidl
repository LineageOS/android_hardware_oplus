/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.NetworkClusterHoRecord;

@VintfStability
parcelable NetworkClusterHoCfg {
    char num;
    char mcc;
    NetworkClusterHoRecord[] hoData;
}
