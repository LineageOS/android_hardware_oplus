/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.ActionCellInfo;

@VintfStability
parcelable ActionCellConfig {
    int expiryTimeSec = 0;
    boolean resetFlag = false;
    boolean cellTypeValid = false;
    int cellType = 0;
    boolean lteCellValid = false;
    ActionCellInfo[] lteCellList;
    boolean nr5gCellValid = false;
    ActionCellInfo[] nr5gCellList;
}
