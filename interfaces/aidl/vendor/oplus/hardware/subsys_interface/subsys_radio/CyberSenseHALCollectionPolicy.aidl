/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys_radio;

@VintfStability
parcelable CyberSenseHALCollectionPolicy {
    int collectionCommand = 0;
    int collectionTimeInterval = 0;
    int reportTimeInterval = 0;
    int collectionDelayTime = 0;
    int collectionWindowTime = 0;
    int collectionGapTime = 0;
    int reportAtScreenOffCommand = 0;
    int collectionCardType = 0;
}
