/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.osense.client;

import vendor.oplus.hardware.osense.client.OsenseCpuControlData;
import vendor.oplus.hardware.osense.client.OsenseCpuMIGData;
import vendor.oplus.hardware.osense.client.OsenseGpuControlData;

@VintfStability
parcelable OsenseControlInfo {
    int cpu_cluster_num;
    int gpu_cluster_num;
    OsenseCpuControlData[] cpuParam1;
    OsenseGpuControlData[] param1;
    OsenseCpuMIGData[] cpu_mig_data;
    int control_mask;
}
