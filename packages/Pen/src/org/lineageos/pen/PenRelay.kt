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
import android.util.Log

class PenRelay(private val context: Context) {
    private val bluetoothManager by lazy { context.getSystemService(BluetoothManager::class.java) }

    private val thread = HandlerThread(TAG, Process.THREAD_PRIORITY_URGENT_DISPLAY)
    private val handler by lazy { Handler(thread.looper) }

    private val touchHal = TouchHal { info -> session?.sendCommand(info) }

    private val probing = mutableMapOf<String, PenGattSession>()

    @Volatile private var session: PenGattSession? = null

    private var hidHost: BluetoothHidHost? = null

    private val hidListener =
        object : BluetoothProfile.ServiceListener {
            override fun onServiceConnected(profile: Int, proxy: BluetoothProfile) {
                hidHost = proxy as BluetoothHidHost
                proxy.connectedDevices.forEach { device -> handler.post { probe(device) } }
            }

            override fun onServiceDisconnected(profile: Int) {
                hidHost = null
            }
        }

    private val receiver =
        object : BroadcastReceiver() {
            override fun onReceive(context: Context, intent: Intent) {
                if (intent.action == BluetoothAdapter.ACTION_STATE_CHANGED) {
                    val state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, -1)
                    if (state == BluetoothAdapter.STATE_TURNING_OFF) {
                        closeAll()
                    }
                    return
                }
                val device =
                    intent.getParcelableExtra(
                        BluetoothDevice.EXTRA_DEVICE,
                        BluetoothDevice::class.java,
                    ) ?: return
                val state = intent.getIntExtra(BluetoothProfile.EXTRA_STATE, -1)
                if (state == BluetoothProfile.STATE_CONNECTED) {
                    handler.post { probe(device) }
                }
            }
        }

    fun start() {
        thread.start()
        bluetoothManager.adapter?.getProfileProxy(context, hidListener, BluetoothProfile.HID_HOST)
        context.registerReceiver(
            receiver,
            IntentFilter().apply {
                addAction(BluetoothHidHost.ACTION_CONNECTION_STATE_CHANGED)
                addAction(BluetoothAdapter.ACTION_STATE_CHANGED)
            },
            null,
            handler,
            Context.RECEIVER_EXPORTED,
        )
    }

    fun stop() {
        context.unregisterReceiver(receiver)
        hidHost?.let { bluetoothManager.adapter?.closeProfileProxy(BluetoothProfile.HID_HOST, it) }
        handler.post { closeAll() }
        thread.quitSafely()
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
        val address = device.address
        if (session?.device?.address == address || probing.containsKey(address)) {
            return
        }
        val newSession =
            PenGattSession(context, device, touchHal, handler, ::onSessionReady, ::onSessionClosed)
        probing[address] = newSession
        newSession.open()
    }

    private fun onSessionReady(ready: PenGattSession) {
        probing.remove(ready.device.address)
        session?.takeIf { it !== ready }?.close()
        session = ready
    }

    private fun onSessionClosed(closed: PenGattSession) {
        probing.remove(closed.device.address)
        if (session === closed) {
            session = null
        }
    }

    companion object {
        private const val TAG = "OplusPenRelay"
    }
}
