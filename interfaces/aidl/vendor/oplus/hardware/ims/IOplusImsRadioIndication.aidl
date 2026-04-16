/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.ims;

@VintfStability
interface IOplusImsRadioIndication {
    oneway void oemCommonInd(String data);
}
