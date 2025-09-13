/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.urcc;

import vendor.oplus.hardware.urcc.UrccRequestData;

@VintfStability
interface IUrccCallback {
    void onCallback(in UrccRequestData[] urccRequestDataArr);
}
