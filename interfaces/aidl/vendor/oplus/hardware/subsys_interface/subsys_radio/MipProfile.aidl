/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable MipProfile {
    byte[] nai;
    byte naiLength;
    byte revTunPref;
    int mnHaSpi = 0;
    int mnAaaSpi = 0;
    int homeAddress = 0;
    int homeAgentPriv = 0;
    int homeAgentSec = 0;
}
