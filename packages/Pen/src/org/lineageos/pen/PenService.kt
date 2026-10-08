/*
 * SPDX-FileCopyrightText: 2025-2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.PendingIntent
import android.app.Service
import android.bluetooth.BluetoothManager
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanFilter
import android.bluetooth.le.ScanResult
import android.bluetooth.le.ScanSettings
import android.content.Intent
import android.hardware.display.DisplayManager
import android.hardware.input.InputManager
import android.os.Handler
import android.os.IBinder
import android.os.UEventObserver
import android.util.Log
import android.view.Display

class PenService : Service() {
    private val bluetoothManager by lazy { getSystemService(BluetoothManager::class.java) }
    private val displayManager by lazy { getSystemService(DisplayManager::class.java) }
    private val inputManager by lazy { getSystemService(InputManager::class.java) }
    private val notificationManager by lazy { getSystemService(NotificationManager::class.java) }

    private val penSupportedRefreshRate by lazy {
        getString(R.string.config_penSupportedRefreshRate).toFloatOrNull()
    }

    private val handler by lazy { Handler(mainLooper) }

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
                        "0" -> notificationManager.cancel(NOTIFICATION_ID)
                        "1" -> postNotification(pencilAddr)
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
        intent?.getStringExtra(EXTRA_PENCIL_ADDR)?.let { bondBtDevice(it) }

        return START_STICKY
    }

    override fun onBind(intent: Intent?): IBinder? = null

    override fun onCreate() {
        super.onCreate()

        if (penSupportedRefreshRate != null) {
            updateRefreshRateCap()
            inputManager.registerInputDeviceListener(inputObserver, handler)
        }

        observer.startObserving("DEVPATH=/devices/virtual/oplus_wireless/pencil")
    }

    override fun onDestroy() {
        super.onDestroy()

        if (penSupportedRefreshRate != null) {
            inputManager.unregisterInputDeviceListener(inputObserver)
            displayManager.requestMaxRefreshRate(Display.DEFAULT_DISPLAY, 0f)
        }

        observer.stopObserving()
    }

    private fun bondBtDevice(pencilAddr: String) {
        val adapter = bluetoothManager.adapter
        @Suppress("DEPRECATION") adapter.enable()

        val scanner = run {
            repeat(50) {
                adapter.bluetoothLeScanner?.let {
                    return@run it
                }
                Thread.sleep(100)
            }
            return@run null
        }
        scanner?.startScan(
            listOf(ScanFilter.Builder().setDeviceAddress(pencilAddr).build()),
            ScanSettings.Builder()
                .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY)
                .setReportDelay(0L)
                .build(),
            object : ScanCallback() {
                override fun onScanResult(callbackType: Int, result: ScanResult) {
                    super.onScanResult(callbackType, result)
                    scanner.stopScan(this)

                    result.device.createBond()
                }

                override fun onBatchScanResults(results: MutableList<ScanResult>) {
                    super.onBatchScanResults(results)
                    scanner.stopScan(this)
                }

                override fun onScanFailed(errorCode: Int) {
                    super.onScanFailed(errorCode)
                    scanner.stopScan(this)
                }
            },
        )
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
            if (isPenConnected) maxRefreshRate else 0f,
        )
    }

    private fun postNotification(pencilAddr: String) {
        val adapter = bluetoothManager.adapter

        if (adapter.bondedDevices.contains(adapter.getRemoteDevice(pencilAddr))) {
            Log.e(TAG, "$pencilAddr already bonded, bailing out")
            return
        }

        if (notificationManager.getNotificationChannel(NOTIFICATION_CHANNEL_ID) == null) {
            notificationManager.createNotificationChannel(
                NotificationChannel(
                    NOTIFICATION_CHANNEL_ID,
                    NOTIFICATION_CHANNEL_ID,
                    NotificationManager.IMPORTANCE_HIGH,
                )
            )
        }

        val contentIntent =
            PendingIntent.getService(
                this,
                0,
                Intent(this, PenService::class.java).apply {
                    putExtra(EXTRA_PENCIL_ADDR, pencilAddr)
                },
                PendingIntent.FLAG_IMMUTABLE or PendingIntent.FLAG_UPDATE_CURRENT,
            )

        val notification =
            Notification.Builder(this, NOTIFICATION_CHANNEL_ID)
                .setSmallIcon(R.drawable.ic_stylus)
                .setContentTitle(getString(R.string.pen_attached))
                .setContentText(getString(R.string.tap_to_connect))
                .setContentIntent(contentIntent)
                .setAutoCancel(true)
                .build()
        notificationManager.notify(NOTIFICATION_ID, notification)
    }

    companion object {
        private const val TAG = "OplusPenService"

        private const val EXTRA_PENCIL_ADDR = "pencil_addr"

        private const val NOTIFICATION_CHANNEL_ID = "OplusPen"
        private const val NOTIFICATION_ID = 1000
    }
}
