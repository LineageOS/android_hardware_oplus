/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.osense.client;

import vendor.oplus.hardware.osense.client.OsenseDataRange;

@VintfStability
parcelable OsenseCpuControlData {
    OsenseDataRange core;
    OsenseDataRange freq;
    int control_type;
}
