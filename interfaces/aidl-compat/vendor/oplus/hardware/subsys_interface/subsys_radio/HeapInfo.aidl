/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

import vendor.oplus.hardware.subsys_interface.subsys_radio.TaskInfo;

@VintfStability
parcelable HeapInfo {
    String heapName;
    int totalBytes = 0;
    int usedBytes = 0;
    int heapUsedBytes = 0;
    int maxUsed = 0;
    int maxRequest = 0;
    TaskInfo[] taskList;
}
