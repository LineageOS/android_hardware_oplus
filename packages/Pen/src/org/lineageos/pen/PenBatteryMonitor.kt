/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.bluetooth.BluetoothDevice
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.Handler
import java.text.NumberFormat

class PenBatteryMonitor(
    private val context: Context,
    private val handler: Handler,
    private val onLevelChanged: (BluetoothDevice, Int) -> Unit,
    private val onLowBattery: (BluetoothDevice, Int) -> Unit,
) {
    private val notificationManager by lazy {
        context.getSystemService(NotificationManager::class.java)
    }

    private val prefs by lazy { context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE) }

    var isAttached = false
        set(value) {
            field = value
            if (value) {
                notificationManager.cancel(NOTIFICATION_ID)
            }
        }

    var penAddress: String?
        get() = prefs.getString(KEY_PEN_ADDRESS, null)
        set(value) = prefs.edit().putString(KEY_PEN_ADDRESS, value).apply()

    private var isLowBatteryWarned: Boolean
        get() = prefs.getBoolean(KEY_LOW_BATTERY_WARNED, false)
        set(value) = prefs.edit().putBoolean(KEY_LOW_BATTERY_WARNED, value).apply()

    private val receiver =
        object : BroadcastReceiver() {
            override fun onReceive(context: Context, intent: Intent) {
                val device =
                    intent.getParcelableExtra(
                        BluetoothDevice.EXTRA_DEVICE,
                        BluetoothDevice::class.java,
                    ) ?: return
                if (!device.address.equals(penAddress, ignoreCase = true)) {
                    return
                }
                updateLevel(
                    device,
                    intent.getIntExtra(
                        BluetoothDevice.EXTRA_BATTERY_LEVEL,
                        BluetoothDevice.BATTERY_LEVEL_UNKNOWN,
                    ),
                )
            }
        }

    fun start() {
        context.registerReceiver(
            receiver,
            IntentFilter(BluetoothDevice.ACTION_BATTERY_LEVEL_CHANGED),
            null,
            handler,
            Context.RECEIVER_EXPORTED,
        )
    }

    fun stop() {
        context.unregisterReceiver(receiver)
    }

    private fun updateLevel(device: BluetoothDevice, level: Int) {
        if (level !in 0..100) {
            return
        }
        onLevelChanged(device, level)

        if (level > LOW_BATTERY_LEVEL) {
            if (isLowBatteryWarned) {
                isLowBatteryWarned = false
                notificationManager.cancel(NOTIFICATION_ID)
            }
            return
        }

        if (isAttached || isLowBatteryWarned) {
            return
        }
        isLowBatteryWarned = true
        postLowBatteryNotification(level)
        onLowBattery(device, level)
    }

    private fun postLowBatteryNotification(level: Int) {
        if (notificationManager.getNotificationChannel(NOTIFICATION_CHANNEL_ID) == null) {
            notificationManager.createNotificationChannel(
                NotificationChannel(
                    NOTIFICATION_CHANNEL_ID,
                    context.getString(R.string.pen_battery_channel_name),
                    NotificationManager.IMPORTANCE_HIGH,
                )
            )
        }

        val percentage = NumberFormat.getPercentInstance().format(level / 100.0)
        val notification =
            Notification.Builder(context, NOTIFICATION_CHANNEL_ID)
                .setSmallIcon(R.drawable.ic_stylus)
                .setContentTitle(context.getString(R.string.pen_battery_low_title))
                .setContentText(context.getString(R.string.pen_battery_low_text, percentage))
                .setCategory(Notification.CATEGORY_STATUS)
                .setOnlyAlertOnce(true)
                .setLocalOnly(true)
                .setAutoCancel(true)
                .build()
        notificationManager.notify(NOTIFICATION_ID, notification)
    }

    companion object {
        // Matches SystemUI's USI stylus low battery threshold
        private const val LOW_BATTERY_LEVEL = 16

        private const val NOTIFICATION_CHANNEL_ID = "OplusPenBattery"
        private const val NOTIFICATION_ID = 1001

        private const val PREFS_NAME = "pen_battery"
        private const val KEY_PEN_ADDRESS = "pen_address"
        private const val KEY_LOW_BATTERY_WARNED = "low_battery_warned"
    }
}
