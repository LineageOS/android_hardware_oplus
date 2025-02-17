/*
 * Copyright (C) 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package com.oplus.internal.telephony.radio.aidl;

import android.content.Context;
import android.telephony.Rlog;

public class QtiRadioConfigAidlProxy {
    private int mCurrentAidlVersion;

    private static final int FEATURE_SMART_TEMP_DDS_VIA_RADIO_CONFIG = 101;

    public static QtiRadioConfigAidlProxy sInstance = null;

    public QtiRadioConfigAidlProxy(Context context) {
        mCurrentAidlVersion = -1;
    }

    public static QtiRadioConfigAidlProxy getInstance(Context context) {
        if (sInstance == null) {
            sInstance = new QtiRadioConfigAidlProxy(context);
        }
        return sInstance;
    }

    public boolean isFeatureSupported(int i) {
        if (i != FEATURE_SMART_TEMP_DDS_VIA_RADIO_CONFIG || mCurrentAidlVersion < 5) {
            return false;
        }
        Rlog.d("QtiRadioConfigAidl", "FEATURE_SMART_TEMP_DDS_VIA_RADIO_CONFIG feature supported");
        return true;
    }
}
