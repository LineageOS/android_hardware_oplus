/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable McfgRemoteDiscoverInfo {
    int error = 0;
    byte remoteDiscoveryInfoValid;
    int hwMbnDiscoveryInfo = 0;
    int swMbnDiscoveryInfo = 0;
    byte ind_token_valid;
    int ind_token = 0;
}
