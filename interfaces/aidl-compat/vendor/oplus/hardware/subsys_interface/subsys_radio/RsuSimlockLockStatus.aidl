/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable RsuSimlockLockStatus {
    int category = 0;
    byte categoryStatus = 0;
    byte blacklist = 0;
    int numRetriesMax = 0;
    int currRetries = 0;
    byte categoryOkToReactivate = 0;
    byte ckPresent = 0;
    byte isPersoEverLocked = 0;
}
