/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.communicationcenter;

@Backing(type="int")
@VintfStability
enum DmtpErrorCode {
    UNKNOWN_ERROR = -5,
    ERR_INVAILD_PARAM = -4,
    ERR_INVAILD_OPERATE = -3,
    ERR_UNSUPPORTED = -2,
    ERR_SERVICE_UNAVAILABLE = -1,
    NO_ERR = 0,
    ERR_URL = 100,
    ERR_SERVICE_DISCONNECTED = 200,
    ERR_TRANSFER_TIMEOUT = 400,
}
