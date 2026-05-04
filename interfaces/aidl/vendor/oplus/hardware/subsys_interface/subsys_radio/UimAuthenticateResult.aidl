/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable UimAuthenticateResult {
    byte auth_state;
    char auth_content_len;
    byte[] auth_content;
}
