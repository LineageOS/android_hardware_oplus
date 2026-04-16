/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.cammidasservice;

import vendor.oplus.hardware.cammidasservice.PowerFactor;
import vendor.oplus.hardware.cammidasservice.ExplorerInfo;

@VintfStability
interface IMIDASService {
    void setPowerFactor(in PowerFactor powerFactor);
    PowerFactor[] getPowerFactor();
    void setExplorerInfo(in ExplorerInfo explorerInfo);
    ExplorerInfo[] getExplorerInfo();
}
