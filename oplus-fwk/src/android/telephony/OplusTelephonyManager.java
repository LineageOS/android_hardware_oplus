/*
 * Copyright (C) 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package android.telephony;

import android.content.Context;

public class OplusTelephonyManager {
    private static OplusTelephonyManager sInstance = null;

    public OplusTelephonyManager(Context context) {}

    public static OplusTelephonyManager getInstance(Context context) {
        if (sInstance == null) {
            sInstance = new OplusTelephonyManager(context);
        }
        return sInstance;
    }

    public String getImsType(int slotId) {
        return null;
    }
}
