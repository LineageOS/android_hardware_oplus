/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@Backing(type="byte")
@VintfStability
enum TrafficDirection {
    UPLINK = 0,
    DOWNLINK = 1,
}
