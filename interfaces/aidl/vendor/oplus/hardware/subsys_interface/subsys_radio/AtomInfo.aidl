/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.AtomMaskConfig;
import vendor.oplus.hardware.subsys_interface.subsys_radio.AtomParamConfig;

@VintfStability
parcelable AtomInfo {
    AtomMaskConfig[] mask;
    AtomParamConfig[] para;
}
