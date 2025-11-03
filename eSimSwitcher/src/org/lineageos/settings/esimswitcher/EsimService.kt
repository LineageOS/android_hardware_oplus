/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher

import android.app.Service
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.IBinder
import android.util.Log

class EsimService : Service() {
    private val shutdownReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context, intent: Intent) {
            Log.d(TAG, "Received ACTION_SHUTDOWN broadcast")

            val controller = EsimController(context)
            if (controller.hasSN220Chipset()) {
                controller.setUimPower(0)
            }
        }
    }

    override fun onCreate() {
        super.onCreate()
        Log.d(TAG, "EsimService created")

        val filter = IntentFilter(Intent.ACTION_SHUTDOWN)
        registerReceiver(shutdownReceiver, filter)
    }

    override fun onDestroy() {
        super.onDestroy()
        unregisterReceiver(shutdownReceiver)
    }

    override fun onBind(intent: Intent?): IBinder? = null

    companion object {
        private const val TAG = "OplusEsimService"
    }
}
