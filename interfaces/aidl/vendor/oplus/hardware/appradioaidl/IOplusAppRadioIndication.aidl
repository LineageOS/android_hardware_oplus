/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.appradioaidl;

@VintfStability
interface IOplusAppRadioIndication {
    oneway void onNecIndication(in byte[] payload);
    oneway void onEventIndication(in String[] payload);
}
