/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.NetworkClusterHoCfg;
import vendor.oplus.hardware.subsys_interface.subsys_radio.NetworkClusterRedirCfg;

@VintfStability
parcelable NetworkClusterCfg {
    byte type = 0;
    NetworkClusterHoCfg hoCfg;
    NetworkClusterRedirCfg redirCfg;
}
