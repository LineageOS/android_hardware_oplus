/*
 * Copyright (C) 2021-2022 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.doze

import android.app.Service
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.IBinder
import android.util.Log

class DozeService : Service() {
    private lateinit var pickupSensor: PickupSensor
    private lateinit var pocketSensor: PocketSensor

    private val screenStateReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context, intent: Intent) {
            when (intent.action) {
                Intent.ACTION_SCREEN_ON -> onDisplayOn()
                Intent.ACTION_SCREEN_OFF -> onDisplayOff()
            }
        }
    }

    override fun onCreate() {
        Log.d(TAG, "Creating service")
        pickupSensor = PickupSensor(
            this,
            resources.getString(R.string.pickup_sensor_type),
            resources.getFloat(R.dimen.pickup_sensor_value),
        )
        pocketSensor = PocketSensor(
            this,
            resources.getString(R.string.pocket_sensor_type),
            resources.getFloat(R.dimen.pocket_sensor_value)
        )

        val screenStateFilter = IntentFilter()
        screenStateFilter.addAction(Intent.ACTION_SCREEN_ON)
        screenStateFilter.addAction(Intent.ACTION_SCREEN_OFF)
        registerReceiver(screenStateReceiver, screenStateFilter)
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        return START_STICKY
    }

    override fun onDestroy() {
        super.onDestroy()

        unregisterReceiver(screenStateReceiver)
        pickupSensor.disable()
        pocketSensor.disable()
    }

    override fun onBind(intent: Intent?): IBinder? = null

    private fun onDisplayOn() {
        if (Utils.isPickUpEnabled(this)) {
            pickupSensor.disable()
        }
        if (Utils.isPocketEnabled(this)) {
            pocketSensor.disable()
        }
    }

    private fun onDisplayOff() {
        if (Utils.isPickUpEnabled(this)) {
            pickupSensor.enable()
        }
        if (Utils.isPocketEnabled(this)) {
            pocketSensor.enable()
        }
    }

    companion object {
        private const val TAG = "DozeService"
    }
}
