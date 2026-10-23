/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable OosCfgInfo {
    boolean enable = false;
    int oosCfgOption = 0;
    int recoveryThreshold = 0;
    int inactiveNwselOosTimer = 0;
    int inactiveSnifferTimer = 0;
    int inactiveModeState = 0;
    int inactiveGmssScanTimer = 0;
    int inactiveGmssSleepTimer = 0;
    int recoveryTimerLv1 = 0;
    int recoveryTimerLv2 = 0;
    int recoveryTimerLv3 = 0;
    int recoveryTimerLv4 = 0;
    int recoveryTimerLv5 = 0;
}
