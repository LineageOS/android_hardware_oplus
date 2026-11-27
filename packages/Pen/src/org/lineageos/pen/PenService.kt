/*
 * SPDX-FileCopyrightText: 2025-2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import android.app.Service
import android.content.Intent
import android.hardware.display.DisplayManager
import android.hardware.input.InputManager
import android.os.Handler
import android.os.IBinder
import android.view.Display

class PenService : Service() {
    private val displayManager by lazy { getSystemService(DisplayManager::class.java) }
    private val inputManager by lazy { getSystemService(InputManager::class.java) }

    private val penSupportedRefreshRate by lazy {
        getString(R.string.config_penSupportedRefreshRate).toFloatOrNull()
    }

    private val penRelay by lazy {
        if (resources.getBoolean(R.bool.config_penPressureRelay)) {
            PenRelay(this) { isActive -> handler.post { onPencilStatusChanged(isActive) } }
        } else {
            null
        }
    }

    private val handler by lazy { Handler(mainLooper) }

    private var pencilStatus: Boolean? = null
    private var isActiveAckSent = false

    private val displayListener =
        object : DisplayManager.DisplayListener {
            override fun onDisplayAdded(displayId: Int) {
                // Do nothing
            }

            override fun onDisplayRemoved(displayId: Int) {
                // Do nothing
            }

            override fun onDisplayChanged(displayId: Int) {
                if (displayId == Display.DEFAULT_DISPLAY) {
                    ackActivePencilIfNeeded()
                }
            }
        }

    private val inputObserver =
        object : InputManager.InputDeviceListener {
            override fun onInputDeviceAdded(deviceId: Int) {
                updateRefreshRateCap()
            }

            override fun onInputDeviceRemoved(deviceId: Int) {
                updateRefreshRateCap()
            }

            override fun onInputDeviceChanged(deviceId: Int) {
                // Do nothing
            }
        }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int) = START_STICKY

    override fun onBind(intent: Intent?): IBinder? = null

    override fun onCreate() {
        super.onCreate()

        if (penSupportedRefreshRate != null) {
            updateRefreshRateCap()
            inputManager.registerInputDeviceListener(inputObserver, handler)
        }

        penRelay?.let {
            displayManager.registerDisplayListener(
                displayListener,
                handler,
                DisplayManager.EVENT_TYPE_DISPLAY_REFRESH_RATE,
            )
            it.start()
        }
    }

    override fun onDestroy() {
        super.onDestroy()

        if (penSupportedRefreshRate != null) {
            inputManager.unregisterInputDeviceListener(inputObserver)
            displayManager.requestMaxRefreshRate(Display.DEFAULT_DISPLAY, 0f)
        }

        penRelay?.let {
            displayManager.unregisterDisplayListener(displayListener)
            it.stop()
        }
    }

    private fun onPencilStatusChanged(isActive: Boolean?) {
        if (pencilStatus != isActive) {
            pencilStatus = isActive
            updateRefreshRateCap()
        }
        isActiveAckSent = false
        when (isActive) {
            true -> ackActivePencilIfNeeded()
            false -> penRelay?.sendPencilStatusAck(false)
            null -> {}
        }
    }

    private fun ackActivePencilIfNeeded() {
        if (pencilStatus != true || isActiveAckSent) {
            return
        }
        val rate = displayManager.getDisplay(Display.DEFAULT_DISPLAY)?.refreshRate ?: return
        val maxRate = penSupportedRefreshRate ?: Float.POSITIVE_INFINITY
        if (rate > maxRate + REFRESH_RATE_TOLERANCE) {
            return
        }
        isActiveAckSent = true
        penRelay?.sendPencilStatusAck(true)
    }

    private fun updateRefreshRateCap() {
        val maxRefreshRate = penSupportedRefreshRate ?: return

        val isPenConnected =
            inputManager.inputDeviceIds.firstOrNull {
                val device = inputManager.getInputDevice(it) ?: return@firstOrNull false
                if (device.vendorId != 0x22D9 && device.vendorId != 0x330A) {
                    // Not an OPPO/Maxeye vendor ID
                    return@firstOrNull false
                }
                if (
                    device.bluetoothAddress?.startsWith("C0:87:06") == false &&
                        device.bluetoothAddress?.startsWith("F8:6F:DE") == false
                ) {
                    // Not a Maxeye/Goodix MAC prefix
                    return@firstOrNull false
                }
                return@firstOrNull true
            } != null

        displayManager.requestMaxRefreshRate(
            Display.DEFAULT_DISPLAY,
            if (pencilStatus ?: isPenConnected) maxRefreshRate else 0f,
        )
    }

    companion object {
        private const val REFRESH_RATE_TOLERANCE = 0.5f
    }
}
