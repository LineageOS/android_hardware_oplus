/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable SmartIdleParaConfig {
    int type = 0;
    int nrTimer = 0;
    int nrCntPerH = 0;
    int lteTimer = 0;
    int lteCntPerH = 0;
}
