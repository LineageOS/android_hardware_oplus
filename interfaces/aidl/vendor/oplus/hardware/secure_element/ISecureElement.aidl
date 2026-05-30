
/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.secure_element;

@VintfStability
interface ISecureElement {
    int is_se_broken();

    boolean is_device_locked();

    byte[] symmetric_crypto(in byte[] data, in byte[] private_data, int key_ref, int cmd);
}
