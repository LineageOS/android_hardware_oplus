/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.touchmonitor

import android.app.Application
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.hardware.display.DisplayManager
import android.os.BatteryManager
import android.os.Handler
import android.os.HandlerThread
import android.view.Display
import android.view.Surface

class TouchMonitorApp : Application() {
    private val displayManager by lazy { getSystemService(DisplayManager::class.java) }
    private val handler by lazy { Handler(HandlerThread(TAG).apply { start() }.looper) }
    private val touchHal = TouchHal { handler.post(::writeAll) }

    private var plugType: Int? = null
    private var rotation: Int? = null

    private val batteryReceiver =
        object : BroadcastReceiver() {
            override fun onReceive(context: Context, intent: Intent) {
                val newPlugType = intent.getIntExtra(BatteryManager.EXTRA_PLUGGED, 0)
                if (newPlugType != plugType) {
                    plugType = newPlugType
                    writePlugType(newPlugType)
                }
            }
        }

    private val displayListener =
        object : DisplayManager.DisplayListener {
            override fun onDisplayAdded(displayId: Int) {}

            override fun onDisplayRemoved(displayId: Int) {}

            override fun onDisplayChanged(displayId: Int) {
                if (displayId == Display.DEFAULT_DISPLAY) {
                    updateRotation()
                }
            }
        }

    override fun onCreate() {
        super.onCreate()
        handler.post {
            updateRotation()
            displayManager.registerDisplayListener(displayListener, handler)
            registerReceiver(
                batteryReceiver,
                IntentFilter(Intent.ACTION_BATTERY_CHANGED),
                null,
                handler,
            )
        }
    }

    private fun updateRotation() {
        val newRotation = displayManager.getDisplay(Display.DEFAULT_DISPLAY)?.rotation ?: return
        if (newRotation != rotation) {
            rotation = newRotation
            writeRotation(newRotation)
        }
    }

    private fun writeAll() {
        plugType?.let(::writePlugType)
        rotation?.let(::writeRotation)
    }

    private fun writePlugType(plugType: Int) {
        val wired =
            plugType == BatteryManager.BATTERY_PLUGGED_AC ||
                plugType == BatteryManager.BATTERY_PLUGGED_USB
        val wireless = plugType == BatteryManager.BATTERY_PLUGGED_WIRELESS
        touchHal.writeNode(CHARGE_DETECT_NODE, if (wired) "1" else "0")
        touchHal.writeNode(WIRELESS_CHARGE_DETECT_NODE, if (wireless) "1" else "0")
    }

    private fun writeRotation(rotation: Int) {
        val (direction, limitEnable) =
            when (rotation) {
                Surface.ROTATION_90 -> "1" to "0"
                Surface.ROTATION_270 -> "2" to "0"
                else -> "0" to "1"
            }
        touchHal.writeNode(LIMIT_WHITELIST_NODE, "0")
        touchHal.writeNode(DIRECTION_NODE, direction)
        touchHal.writeNode(LIMIT_ENABLE_NODE, limitEnable)
    }

    companion object {
        private const val TAG = "OplusTouchMonitor"

        private const val DIRECTION_NODE = 5
        private const val LIMIT_WHITELIST_NODE = 6
        private const val LIMIT_ENABLE_NODE = 7
        private const val CHARGE_DETECT_NODE = 10
        private const val WIRELESS_CHARGE_DETECT_NODE = 11
    }
}
