/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable CyberSenseHalBeamInfo {
    int ssbIndex = 0;
    int beam_rsrp = 0;
    int beam_rsrq = 0;
}
