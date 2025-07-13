/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

@VintfStability
parcelable CarrierIdentifier {
    String mccmnc;
    String imsi;
    String iccid;
    String gid1;
    String gid2;
    String spn;
}
