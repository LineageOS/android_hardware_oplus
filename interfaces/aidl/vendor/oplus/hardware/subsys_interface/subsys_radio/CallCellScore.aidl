/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable CallCellScore {
    int score = 0;
    int sinrScore = 0;
    int pagingScore = 0;
    int pdcpScore = 0;
    int rlfScore = 0;
    int rrcScore = 0;
}
