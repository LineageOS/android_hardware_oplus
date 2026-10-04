/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothGatt
import android.bluetooth.BluetoothGattCallback
import android.bluetooth.BluetoothGattCharacteristic
import android.bluetooth.BluetoothGattConnectionSettings
import android.bluetooth.BluetoothGattDescriptor
import android.bluetooth.BluetoothProfile
import android.bluetooth.BluetoothStatusCodes
import android.os.Handler
import android.util.Log

class PenGattSession(
    val device: BluetoothDevice,
    private val touchHal: TouchHal,
    private val handler: Handler,
    private val onReady: (PenGattSession) -> Unit,
    private val onClosed: (PenGattSession) -> Unit,
) {
    private var gatt: BluetoothGatt? = null
    private var pressChar: BluetoothGattCharacteristic? = null
    private var speedChar: BluetoothGattCharacteristic? = null

    private var isReady = false
    private var isClosed = false
    private var pressPacketCount = 0

    private val pendingWrites = ArrayDeque<Pair<BluetoothGattCharacteristic, ByteArray>>()
    private var isWriteInFlight = false

    private val callback =
        object : BluetoothGattCallback() {
            override fun onConnectionStateChange(gatt: BluetoothGatt, status: Int, newState: Int) {
                when (newState) {
                    BluetoothProfile.STATE_CONNECTED -> {
                        if (!gatt.discoverServices()) {
                            Log.e(TAG, "discoverServices failed")
                            close()
                        }
                    }
                    BluetoothProfile.STATE_DISCONNECTED -> close()
                }
            }

            override fun onServicesDiscovered(gatt: BluetoothGatt, status: Int) {
                if (status != BluetoothGatt.GATT_SUCCESS) {
                    Log.e(TAG, "Service discovery failed: $status")
                    close()
                    return
                }
                val service = gatt.getService(PenProtocol.SERVICE_UUID)
                val press = service?.getCharacteristic(PenProtocol.PRESS_CHAR_UUID)
                val ccc = press?.getDescriptor(PenProtocol.CCC_DESCRIPTOR_UUID)
                if (press == null || ccc == null) {
                    Log.d(TAG, "${device.address} has no pen press service")
                    close()
                    return
                }
                pressChar = press
                speedChar = service.getCharacteristic(PenProtocol.SPEED_CHAR_UUID)

                gatt.setCharacteristicNotification(press, true)
                val ret =
                    gatt.writeDescriptor(ccc, BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE)
                if (ret != BluetoothStatusCodes.SUCCESS) {
                    Log.e(TAG, "Failed to enable pen press notifications: $ret")
                    close()
                }
            }

            override fun onDescriptorWrite(
                gatt: BluetoothGatt,
                descriptor: BluetoothGattDescriptor,
                status: Int,
            ) {
                if (status != BluetoothGatt.GATT_SUCCESS) {
                    Log.e(TAG, "CCC write failed: $status")
                    close()
                    return
                }
                if (!gatt.requestMtu(PenProtocol.MTU)) {
                    Log.w(TAG, "requestMtu failed, continuing with default MTU")
                    markReady()
                }
            }

            override fun onMtuChanged(gatt: BluetoothGatt, mtu: Int, status: Int) {
                Log.d(TAG, "MTU changed to $mtu, status $status")
                markReady()
            }

            override fun onCharacteristicChanged(
                gatt: BluetoothGatt,
                characteristic: BluetoothGattCharacteristic,
                value: ByteArray,
            ) {
                if (characteristic.uuid != PenProtocol.PRESS_CHAR_UUID) {
                    return
                }
                if (PenProtocol.isPressurePacket(value)) {
                    if (pressPacketCount == PenProtocol.KEEP_ALIVE_INTERVAL) {
                        pressPacketCount = 0
                        speedChar?.let { enqueueWrite(it, PenProtocol.KEEP_ALIVE) }
                    } else {
                        pressPacketCount++
                    }
                }
                touchHal.writeBtInfo(value.toHexString())
            }

            override fun onCharacteristicWrite(
                gatt: BluetoothGatt,
                characteristic: BluetoothGattCharacteristic,
                status: Int,
            ) {
                if (status != BluetoothGatt.GATT_SUCCESS) {
                    Log.w(TAG, "Write to ${characteristic.uuid} failed: $status")
                }
                isWriteInFlight = false
                drainWrites()
            }
        }

    fun open() {
        Log.d(TAG, "Probing ${device.address}")
        gatt =
            device.connectGatt(
                BluetoothGattConnectionSettings.Builder()
                    .setTransport(BluetoothDevice.TRANSPORT_LE)
                    .setAutomaticMtuEnabled(false)
                    .build(),
                { handler.post(it) },
                callback,
            )
        if (gatt == null) {
            Log.e(TAG, "connectGatt failed for ${device.address}")
            close()
        }
    }

    fun sendCommand(info: String) {
        handler.post {
            if (!isReady) {
                return@post
            }
            val payload = PenProtocol.decodeCommand(info)
            if (payload == null) {
                Log.e(TAG, "Invalid touch command: $info")
                return@post
            }
            val target = if (PenProtocol.isSpeedCommand(payload)) speedChar else pressChar
            target?.let { enqueueWrite(it, payload) }
        }
    }

    fun close() {
        if (isClosed) {
            return
        }
        isClosed = true
        if (isReady) {
            isReady = false
            touchHal.writeBtInfo(PenProtocol.PRESSURE_CLEAR)
            touchHal.writeNode(PenProtocol.NODE_PENCIL_CONNECTED, "0")
            touchHal.unregisterCallback()
            Log.i(TAG, "Pen ${device.address} disconnected")
        }
        pendingWrites.clear()
        gatt?.close()
        gatt = null
        onClosed(this)
    }

    private fun markReady() {
        if (isReady) {
            return
        }
        isReady = true
        pressPacketCount = 0
        touchHal.registerCallback()
        touchHal.writeNode(PenProtocol.NODE_PENCIL_CONNECTED, "0")
        touchHal.writeNode(PenProtocol.NODE_PENCIL_CONNECTED, "1")
        Log.i(TAG, "Pen ${device.address} ready")
        onReady(this)
    }

    private fun enqueueWrite(characteristic: BluetoothGattCharacteristic, value: ByteArray) {
        pendingWrites.addLast(characteristic to value)
        drainWrites()
    }

    private fun drainWrites() {
        val gatt = gatt ?: return
        while (!isWriteInFlight) {
            val (characteristic, value) = pendingWrites.removeFirstOrNull() ?: return
            val noResponse =
                characteristic.properties and
                    BluetoothGattCharacteristic.PROPERTY_WRITE_NO_RESPONSE != 0
            val writeType =
                if (noResponse) {
                    BluetoothGattCharacteristic.WRITE_TYPE_NO_RESPONSE
                } else {
                    BluetoothGattCharacteristic.WRITE_TYPE_DEFAULT
                }
            val ret = gatt.writeCharacteristic(characteristic, value, writeType)
            if (ret == BluetoothStatusCodes.SUCCESS) {
                isWriteInFlight = true
            } else {
                Log.w(TAG, "Dropping write to ${characteristic.uuid}: $ret")
            }
        }
    }

    companion object {
        private const val TAG = "OplusPenGatt"
    }
}
