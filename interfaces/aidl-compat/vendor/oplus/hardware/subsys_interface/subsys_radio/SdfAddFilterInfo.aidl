/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable SdfAddFilterInfo {
    byte[] srcAddr;
    int srcPort = 0;
    byte[] dstAddr;
    int dstPort = 0;
    int protocal = 0;
    int uplinkLevel = 0;
    int downLinkLevel = 0;
    int ood = 0;
    int appType = 0;
    int dscp = 0;
    int pdcpTimer = 0;
    int inactivityTimer = 0;
    int flowLabel = 0;
    int sourcePortRange = 0;
    int destPortRange = 0;
    int sourceIPv6Len = 0;
    int destIPv6Len = 0;
}
