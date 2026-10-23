/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.PowerBwpStatisticsInfo;
import vendor.oplus.hardware.subsys_interface.subsys_radio.PowerSmartIdleStatisticsInfo;

@VintfStability
parcelable PowerStatisticsSpecificInfo {
    PowerSmartIdleStatisticsInfo smartIdleInfo;
    PowerBwpStatisticsInfo bwpInfo;
}
