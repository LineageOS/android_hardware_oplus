/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

@VintfStability
@Backing(type="int")
enum EccCatoryVal {
    ECC_SERVICE_CAT_DEFAULT = 0,
    ECC_SERVICE_CAT_POLICE = 1,
    ECC_SERVICE_CAT_AMBULANCE = 2,
    ECC_SERVICE_CAT_FIRE_BRIGADE = 4,
    ECC_SERVICE_CAT_MARINE_GUARD = 8,
    ECC_SERVICE_CAT_MOUNTAIN_RESCUE = 16,
    ECC_SERVICE_CAT_MANUAL_INIT_ECALL = 32,
    ECC_SERVICE_CAT_AUTOMATIC_INIT_ECALL = 64,
    ECC_SERVICE_CAT_RESERVE = 128
}
