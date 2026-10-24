/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.charger;

@VintfStability
@Backing(type="int")
enum KernelTestResult {
    kPass     = 0,
    kDisabled = 1,
    kFail     = 2,
}
