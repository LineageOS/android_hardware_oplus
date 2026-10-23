/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable SimlockStateType {
    byte stateBeforePolicy = 0;
    byte stateAfterPolicy = 0;
    byte lockState = 0;
}
