/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.charger;

@VintfStability
@Backing(type="int")
enum ChargerErrorCode {
    ERR_FILE_FAILURE_ACCESS = -2147483648,
    ERR_FILE_FAILURE_OPEN   = -2147483647,
    ERR_FILE_FAILURE_READ   = -2147483646,
    ERR_FILE_FAILURE_WRITE  = -2147483645,
    ERR_FILE_NOT_EXIST      = -2147483644,
}
