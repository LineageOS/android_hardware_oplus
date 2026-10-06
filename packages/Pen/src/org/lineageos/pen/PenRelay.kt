/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothHidHost
import android.bluetooth.BluetoothManager
import android.bluetooth.BluetoothProfile
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.Handler
import android.os.HandlerThread
import android.os.Process

class PenRelay(
    private val context: Context,
    private val onPencilStatusChanged: (Boolean?) -> Unit,
) {
    private val bluetoothManager by lazy { context.getSystemService(BluetoothManager::class.java) }

    private val thread = HandlerThread(TAG, Process.THREAD_PRIORITY_URGENT_DISPLAY)
    private val handler by lazy { Handler(thread.looper) }

    private val touchHal = TouchHal { info -> session?.sendCommand(info) }

    private val probing = mutableMapOf<String, PenGattSession>()

    @Volatile private var session: PenGattSession? = null

    private val receiver =
        object : BroadcastReceiver() {
            override fun onReceive(context: Context, intent: Intent) {
                when (intent.action) {
                    BluetoothAdapter.ACTION_STATE_CHANGED -> {
                        val state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, -1)
                        if (state == BluetoothAdapter.STATE_TURNING_OFF) {
                            closeAll()
                        }
                    }
                    BluetoothHidHost.ACTION_CONNECTION_STATE_CHANGED -> {
                        val state = intent.getIntExtra(BluetoothProfile.EXTRA_STATE, -1)
                        if (state == BluetoothProfile.STATE_CONNECTED) {
                            intent
                                .getParcelableExtra(
                                    BluetoothDevice.EXTRA_DEVICE,
                                    BluetoothDevice::class.java,
                                )
                                ?.let { probe(it) }
                        }
                    }
                }
            }
        }

    fun start() {
        thread.start()
        context.registerReceiver(
            receiver,
            IntentFilter().apply {
                addAction(BluetoothAdapter.ACTION_STATE_CHANGED)
                addAction(BluetoothHidHost.ACTION_CONNECTION_STATE_CHANGED)
            },
            null,
            handler,
            Context.RECEIVER_EXPORTED,
        )
        handler.post {
            bluetoothManager.adapter
                ?.bondedDevices
                ?.filter { it.isConnected }
                ?.forEach { probe(it) }
        }
    }

    fun stop() {
        context.unregisterReceiver(receiver)
        handler.post { closeAll() }
        thread.quitSafely()
    }

    fun sendPencilStatusAck(isActive: Boolean) {
        session?.sendPencilStatusAck(isActive)
    }

    // GATT callbacks stop once Bluetooth goes down, so sessions must be dropped explicitly
    private fun closeAll() {
        probing.values.toList().forEach { it.close() }
        session?.close()
    }

    private fun probe(device: BluetoothDevice) {
        if (device.type == BluetoothDevice.DEVICE_TYPE_CLASSIC) {
            return
        }
        if (session?.device == device || probing.containsKey(device.address)) {
            return
        }
        val newSession =
            PenGattSession(
                device,
                touchHal,
                handler,
                ::onSessionReady,
                ::onSessionClosed,
                ::onPencilStatus,
            )
        probing[device.address] = newSession
        newSession.open()
    }

    private fun onSessionReady(ready: PenGattSession) {
        probing.remove(ready.device.address)
        session?.takeIf { it !== ready }?.close()
        session = ready
        onPencilStatusChanged(ready.isPencilActive ?: false)
    }

    private fun onSessionClosed(closed: PenGattSession) {
        probing.remove(closed.device.address)
        if (session === closed) {
            session = null
            onPencilStatusChanged(null)
        }
    }

    private fun onPencilStatus(from: PenGattSession, isActive: Boolean) {
        if (session === from) {
            onPencilStatusChanged(isActive)
        }
    }

    companion object {
        private const val TAG = "OplusPenRelay"
    }
}
