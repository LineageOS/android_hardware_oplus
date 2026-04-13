/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys;

@VintfStability
parcelable IccMetaReqInfo {
    int id = 0;
    int token = 0;
    byte type = 0;
    int reqId = 0;
    byte[] reqData;
    int timeoutMs = 0;
}
