/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.ActionCell;
import vendor.oplus.hardware.subsys_interface.subsys_radio.ActionCellConfig;

@VintfStability
parcelable ActionConfig {
    int clientId = 0;
    int actionId = 0;
    int firstLevel = 0;
    int actionMode = 0;
    int keepTimeMs = 0;
    int cellSrchFlag = 0;
    ActionCell[] cell;
    boolean cellConfigValid = false;
    ActionCellConfig cellConfig;
}
