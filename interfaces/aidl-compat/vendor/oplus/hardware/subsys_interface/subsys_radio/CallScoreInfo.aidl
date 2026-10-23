/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.CallCellInfo;

@VintfStability
parcelable CallScoreInfo {
    int actionType = 0;
    int featureSwitch = 0;
    int weightSinr = 0;
    int weightPaging = 0;
    int weightPdcp = 0;
    int weightRlf = 0;
    int weightRrc = 0;
    int[] updatedId;
    CallCellInfo[] topCells;
}
