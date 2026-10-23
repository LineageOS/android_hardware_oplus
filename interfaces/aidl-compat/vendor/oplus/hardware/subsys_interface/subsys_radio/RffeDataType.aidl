/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable RffeDataType {
    byte ext = 0;
    byte readwrite = 0;
    byte channel = 0;
    byte slave = 0;
    char address;
    byte data = 0;
    byte halfspeed = 0;
}
