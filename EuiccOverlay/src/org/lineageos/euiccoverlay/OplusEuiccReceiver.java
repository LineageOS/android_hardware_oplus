/*
 * Copyright (C) 2023 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.euiccoverlay;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class OplusEuiccReceiver extends BroadcastReceiver {
    private static final String TAG = "OplusEuiccReceiver";

    @Override
    public void onReceive(final Context context, Intent intent) {
        Log.d(TAG, "Received PARTNER_CUSTOMIZATION intent");
    }
}
