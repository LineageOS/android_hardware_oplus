/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.communicationcenter;

@VintfStability
parcelable TransportStats {
    long mTxCount;
    long mRxCount;
    long mLossCount;
    int mCurSpeed;
    long mCurDelay;
    int mNetworkCondition;
}
