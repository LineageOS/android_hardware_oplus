/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable RsuSimlockLockStatus {
    int category = 0;
    byte categoryStatus;
    byte blacklist;
    int numRetriesMax = 0;
    int currRetries = 0;
    byte categoryOkToReactivate;
    byte ckPresent;
    byte isPersoEverLocked;
}
