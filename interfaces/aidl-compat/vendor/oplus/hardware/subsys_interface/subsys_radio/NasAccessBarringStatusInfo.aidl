/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable NasAccessBarringStatusInfo {
    int radioIf = 0;
    byte dataBarringStatus = 0;
    int dataBarringTime = 0;
    byte voiceBarringStatus = 0;
    int voiceBarringTime = 0;
    byte videoBarringStatus = 0;
    int videoBarringTime = 0;
}
