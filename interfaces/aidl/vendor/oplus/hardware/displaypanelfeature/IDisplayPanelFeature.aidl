/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.displaypanelfeature;

@VintfStability
interface IDisplayPanelFeature {
    int getDisplayPanelFeatureValue(int featureId, inout int[] featureValues);
    int setDisplayPanelFeatureValue(int featureId, in int[] featureValues);
    int getDisplayPanelInfo(int featureId, out List<String> panelInfo);
}
