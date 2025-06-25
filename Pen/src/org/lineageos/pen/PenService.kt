/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
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
import android.content.Context
import android.content.Intent
import android.os.IBinder
import android.os.UEventObserver
import android.util.Log

class PenService : Service() {
    private val bluetoothManager by lazy { getSystemService(BluetoothManager::class.java) }
    private val notificationManager by lazy { getSystemService(NotificationManager::class.java) }

    private val observer = object : UEventObserver() {
        private val lock = Any()

        override fun onUEvent(event: UEvent) {
            synchronized(lock) {
                val pencilStatus = event.get("pencil_status") ?: return
                val pencilAddr = event.get("pencil_addr")?.chunked(2)?.joinToString(":") {
                    it.uppercase()
                } ?: return

                when (pencilStatus) {
                    "0" -> notificationManager.cancel(NOTIFICATION_ID)
                    "1" -> postNotification(pencilAddr)
                }
            }
        }
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        intent?.getStringExtra(EXTRA_PENCIL_ADDR)?.let {
            bondBtDevice(it)
        }

        return START_STICKY
    }

    override fun onBind(intent: Intent?): IBinder? = null

    override fun onCreate() {
        super.onCreate()

        observer.startObserving("DEVPATH=/devices/virtual/oplus_wireless/pencil")
    }

    override fun onDestroy() {
        super.onDestroy()

        observer.stopObserving()
    }

    private fun bondBtDevice(pencilAddr: String) {
        val adapter = bluetoothManager.adapter
        @Suppress("DEPRECATION") adapter.enable()

        val scanner = adapter.bluetoothLeScanner
        scanner.startScan(
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
            }
        )
    }

    private fun postNotification(pencilAddr: String) {
        val adapter = bluetoothManager.adapter

        if (adapter.bondedDevices.contains(adapter.getRemoteDevice(pencilAddr))) {
            Log.e(TAG, "$pencilAddr already bonded, bailing out")
            return
        }

        notificationManager.getNotificationChannel(NOTIFICATION_CHANNEL_ID) ?: run {
            notificationManager.createNotificationChannel(
                NotificationChannel(
                    NOTIFICATION_CHANNEL_ID,
                    NOTIFICATION_CHANNEL_ID,
                    NotificationManager.IMPORTANCE_DEFAULT
                )
            )
        }

        val contentIntent = PendingIntent.getService(
            this,
            0,
            Intent(this, PenService::class.java).apply {
                putExtra(EXTRA_PENCIL_ADDR, pencilAddr)
            },
            PendingIntent.FLAG_IMMUTABLE or PendingIntent.FLAG_UPDATE_CURRENT
        )

        val notification = Notification.Builder(this, NOTIFICATION_CHANNEL_ID)
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
