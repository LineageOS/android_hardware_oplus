/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.cammidasservice;

@VintfStability
parcelable ExplorerExceptionInfo {
    int moduleId;
    int majorType;
    int minorType;
    int level;
    int action;
}
