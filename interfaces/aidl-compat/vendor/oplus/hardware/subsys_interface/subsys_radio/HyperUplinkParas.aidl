/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.BlackBandList;
import vendor.oplus.hardware.subsys_interface.subsys_radio.CallPdcpCfg;
import vendor.oplus.hardware.subsys_interface.subsys_radio.FallbackOffsetCfg;
import vendor.oplus.hardware.subsys_interface.subsys_radio.PowerEvaluateCfg;
import vendor.oplus.hardware.subsys_interface.subsys_radio.RarCfg;
import vendor.oplus.hardware.subsys_interface.subsys_radio.SceneEvaluateCfg;

@VintfStability
parcelable HyperUplinkParas {
    PowerEvaluateCfg hyperCommonModeCfg;
    PowerEvaluateCfg islandModeCfg;
    FallbackOffsetCfg fallbackOffsetCfg;
    SceneEvaluateCfg sceneEvaluateCfg;
    CallPdcpCfg callPdcpCfg;
    RarCfg rarCfg;
    BlackBandList blackBandList;
}
