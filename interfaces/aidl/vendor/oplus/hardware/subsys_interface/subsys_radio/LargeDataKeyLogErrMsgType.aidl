/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable LargeDataKeyLogErrMsgType {
    int cmd = 0;
    int len = 0;
    byte[] msg;
}
