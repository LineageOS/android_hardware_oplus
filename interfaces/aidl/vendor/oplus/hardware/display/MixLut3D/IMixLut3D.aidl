/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.display.MixLut3D;

@VintfStability
interface IMixLut3D {
    int set(int panelId, int cmd);

    int sencept(
        int panelId,
        int cmd,
        in int[] vals,
        inout int[] data
    );
}
