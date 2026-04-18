/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.communicationcenter;

@VintfStability
parcelable DmtpFrameWrapper {
    byte mMagicNumb;
    int mDataSize;
    byte mTopicId;
    byte mBusinessId;
    byte mMsgType;
    int mReply;
    int mSerialNumb;
    int mMsgId;
    byte[] data;
}
