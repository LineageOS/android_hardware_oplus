/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.keyboard

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
import android.content.Context
import android.content.Intent
import android.os.IBinder
import android.os.UEventObserver
import android.util.Log

class KeyboardService : Service() {
    private val bluetoothManager by lazy { getSystemService(BluetoothManager::class.java) }
    private val notificationManager by lazy { getSystemService(NotificationManager::class.java) }

    private val observer = object : UEventObserver() {
        private val lock = Any()

        override fun onUEvent(event: UEvent) {
            synchronized(lock) {
                val KeyboardStatus = event.get("pogopin_status") ?: return
                val KeyboardAddr = event.get("mac_addr")?.chunked(2)?.joinToString(":") {
                    it.uppercase()
                } ?: return

                when (KeyboardStatus) {
                    "0" -> notificationManager.cancel(NOTIFICATION_ID)
                    "1" -> postNotification(KeyboardAddr)
                }
            }
        }
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        intent?.getStringExtra(EXTRA_KEYBOARD_ADDR)?.let {
            bondBtDevice(it)
        }

        return START_STICKY
    }

    override fun onBind(intent: Intent?): IBinder? = null

    override fun onCreate() {
        super.onCreate()

        observer.startObserving("DEVPATH=/devices/virtual/pogopin/pogo_keyboard")
    }

    override fun onDestroy() {
        super.onDestroy()

        observer.stopObserving()
    }

    private fun bondBtDevice(KeyboardAddr: String) {
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
            listOf(ScanFilter.Builder().setDeviceAddress(KeyboardAddr).build()),
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
            }
        )
    }

    private fun postNotification(KeyboardAddr: String) {
        val adapter = bluetoothManager.adapter

        if (adapter.bondedDevices.contains(adapter.getRemoteDevice(KeyboardAddr))) {
            Log.e(TAG, "$KeyboardAddr already bonded, bailing out")
            return
        }

        if (notificationManager.getNotificationChannel(NOTIFICATION_CHANNEL_ID) == null) {
            notificationManager.createNotificationChannel(
                NotificationChannel(
                    NOTIFICATION_CHANNEL_ID,
                    NOTIFICATION_CHANNEL_ID,
                    NotificationManager.IMPORTANCE_HIGH
                )
            )
        }

        val contentIntent = PendingIntent.getService(
            this,
            0,
            Intent(this, KeyboardService::class.java).apply {
                putExtra(EXTRA_KEYBOARD_ADDR, KeyboardAddr)
            },
            PendingIntent.FLAG_IMMUTABLE or PendingIntent.FLAG_UPDATE_CURRENT
        )

        val notification = Notification.Builder(this, NOTIFICATION_CHANNEL_ID)
            .setSmallIcon(R.drawable.ic_stylus)
            .setContentTitle(getString(R.string.keyboard_attached))
            .setContentText(getString(R.string.tap_to_connect))
            .setContentIntent(contentIntent)
            .setAutoCancel(true)
            .build()
        notificationManager.notify(NOTIFICATION_ID, notification)
    }

    companion object {
        private const val TAG = "OplusKeyboardService"

        private const val EXTRA_KEYBOARD_ADDR = "mac_addr"

        private const val NOTIFICATION_CHANNEL_ID = "OplusKeyboard"
        private const val NOTIFICATION_ID = 1000
    }
}
