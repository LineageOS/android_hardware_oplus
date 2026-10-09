/*
 * SPDX-FileCopyrightText: 2025-2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import android.app.KeyguardManager
import android.app.Service
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.hardware.display.DisplayManager
import android.hardware.input.InputManager
import android.os.Handler
import android.os.IBinder
import android.os.PowerManager
import android.os.UEventObserver
import android.view.Display

class PenService : Service() {
    private val bluetoothManager by lazy { getSystemService(BluetoothManager::class.java) }
    private val displayManager by lazy { getSystemService(DisplayManager::class.java) }
    private val inputManager by lazy { getSystemService(InputManager::class.java) }
    private val keyguardManager by lazy { getSystemService(KeyguardManager::class.java) }
    private val powerManager by lazy { getSystemService(PowerManager::class.java) }

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

    private val popup by lazy { PenPopup(this) { batteryPopupUpdate = null } }
    private val popupContents by lazy { PenPopupContents(this) }

    private var batteryPopupUpdate: Pair<String, (Int) -> PenPopupContent>? = null

    private var pendingPairAddress: String? = null

    private val batteryMonitor by lazy {
        PenBatteryMonitor(
            this,
            handler,
            onLevelChanged = { device, level ->
                batteryPopupUpdate?.let { (address, update) ->
                    if (address.equals(device.address, ignoreCase = true)) {
                        popup.show(update(level))
                    }
                }
            },
            onLowBattery = { device, level ->
                if (powerManager.isInteractive) {
                    showPopup(popupContents.lowBattery(getPenName(device), level))
                }
            },
        )
    }

    private val pairing by lazy { PenPairing(this, handler, ::onPairingFinished) }

    private val screenOnReceiver =
        object : BroadcastReceiver() {
            override fun onReceive(context: Context, intent: Intent) {
                pendingPairAddress?.let {
                    pendingPairAddress = null
                    showPairPopup(it)
                }
            }
        }

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

    private val observer =
        object : UEventObserver() {
            private val lock = Any()

            override fun onUEvent(event: UEvent) {
                synchronized(lock) {
                    val pencilStatus = event.get("pencil_status") ?: return
                    val pencilAddr =
                        event.get("pencil_addr")?.chunked(2)?.joinToString(":") { it.uppercase() }
                            ?: return

                    when (pencilStatus) {
                        "0" -> handler.post { onPencilDetached() }
                        "1" -> handler.post { onPencilAttached(pencilAddr) }
                    }
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

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        intent?.getStringExtra(EXTRA_PENCIL_ADDR)?.let { startPairing(it) }

        return START_STICKY
    }

    override fun onBind(intent: Intent?): IBinder? = null

    override fun onCreate() {
        super.onCreate()

        if (penSupportedRefreshRate != null) {
            updateRefreshRateCap()
            inputManager.registerInputDeviceListener(inputObserver, handler)
        }

        batteryMonitor.start()
        registerReceiver(
            screenOnReceiver,
            IntentFilter(Intent.ACTION_SCREEN_ON),
            null,
            handler,
            RECEIVER_NOT_EXPORTED,
        )
        observer.startObserving("DEVPATH=/devices/virtual/oplus_wireless/pencil")

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

        observer.stopObserving()
        unregisterReceiver(screenOnReceiver)
        batteryMonitor.stop()
        pairing.stop()
        popup.dismissNow()

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

    private fun onPencilAttached(pencilAddr: String) {
        val device = bluetoothManager.adapter.getRemoteDevice(pencilAddr)
        if (device.bondState != BluetoothDevice.BOND_BONDED) {
            if (powerManager.isInteractive) {
                showPairPopup(pencilAddr)
            } else {
                pendingPairAddress = pencilAddr
            }
            return
        }

        batteryMonitor.penAddress = device.address
        batteryMonitor.isAttached = true
        if (powerManager.isInteractive) {
            showBatteryPopup(device, device.batteryLevel, popupContents::attached)
        }
    }

    private fun onPencilDetached() {
        batteryMonitor.isAttached = false
        pendingPairAddress = null
        if (popup.content?.onClick != null && !pairing.isPairing) {
            popup.dismiss()
        }
    }

    private fun showPairPopup(pencilAddr: String) {
        showPopup(popupContents.pair { onPairTapped(pencilAddr) })
    }

    private fun onPairTapped(pencilAddr: String) {
        if (pairing.isPairing) {
            return
        }
        if (keyguardManager.isKeyguardLocked) {
            popup.dismiss()
            startActivity(
                Intent(this, PenUnlockActivity::class.java).apply {
                    putExtra(EXTRA_PENCIL_ADDR, pencilAddr)
                    addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
                }
            )
            return
        }
        startPairing(pencilAddr)
    }

    private fun startPairing(pencilAddr: String) {
        if (pairing.isPairing) {
            return
        }
        showPopup(popupContents.connecting())
        pairing.start(pencilAddr)
    }

    private fun onPairingFinished(device: BluetoothDevice, isBonded: Boolean) {
        if (!isBonded) {
            showPopup(popupContents.connectFailed { onPairTapped(device.address) })
            return
        }

        batteryMonitor.penAddress = device.address
        batteryMonitor.isAttached = true
        showBatteryPopup(device, device.batteryLevel, popupContents::connected)
    }

    private fun showPopup(content: PenPopupContent) {
        batteryPopupUpdate = null
        popup.show(content)
    }

    private fun showBatteryPopup(
        device: BluetoothDevice,
        level: Int,
        content: (String, Int) -> PenPopupContent,
    ) {
        val name = getPenName(device)
        popup.show(content(name, level))
        batteryPopupUpdate = device.address to { newLevel -> content(name, newLevel) }
    }

    private fun getPenName(device: BluetoothDevice) =
        device.alias ?: getString(R.string.pen_default_name)

    companion object {
        const val EXTRA_PENCIL_ADDR = "pencil_addr"

        private const val REFRESH_RATE_TOLERANCE = 0.5f
    }
}
