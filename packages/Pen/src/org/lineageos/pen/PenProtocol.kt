/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import java.util.UUID

object PenProtocol {
    val SERVICE_UUID: UUID = UUID.fromString("0783b03e-8535-b5a0-7140-a304d2495cb7")
    val PRESS_CHAR_UUID: UUID = UUID.fromString("0783b03e-8535-b5a0-7140-a304d2495cbb")
    val SPEED_CHAR_UUID: UUID = UUID.fromString("0783b03e-8535-b5a0-7140-a304d2495cbc")
    val CCC_DESCRIPTOR_UUID: UUID = UUID.fromString("00002902-0000-1000-8000-00805f9b34fb")

    val INTERFACE_SERVICE_UUID: UUID = UUID.fromString("18092dbc-2a69-11ec-8d3d-0242ac130003")
    val INTERFACE_CHAR_UUID: UUID = UUID.fromString("18093046-2a69-11ec-8d3d-0242ac130003")

    const val MTU = 100

    const val NODE_BT_INFO = 169

    const val NODE_PENCIL_CONNECTED = 32

    const val PRESSURE_CLEAR = "aa0101020000"

    // Every (KEEP_ALIVE_INTERVAL + 1)th pressure packet is acked on the speed characteristic
    const val KEEP_ALIVE_INTERVAL = 5
    val KEEP_ALIVE = byteArrayOf(0xAA.toByte(), 0x02, 0x00, 0x00)

    private const val HEADER = 0xAA.toByte()
    private const val MIN_COMMAND_SIZE = 4

    private val PENCIL_STATUS = byteArrayOf(0x2C, 0x90.toByte(), 0x01, 0x01)

    fun parsePencilStatus(value: ByteArray): Boolean? {
        if (
            value.size <= PENCIL_STATUS.size ||
                !value.copyOf(PENCIL_STATUS.size).contentEquals(PENCIL_STATUS)
        ) {
            return null
        }
        return when (value[PENCIL_STATUS.size]) {
            0x00.toByte() -> false
            0x01.toByte() -> true
            else -> null
        }
    }

    fun pencilStatusAck(isActive: Boolean) = PENCIL_STATUS + (if (isActive) 0x01 else 0x00).toByte()

    fun isPressurePacket(value: ByteArray) =
        value.size >= 4 && value[2] == 0x01.toByte() && value[3] == 0x02.toByte()

    fun toHex(value: ByteArray) = value.joinToString("") { "%02x".format(it) }

    // The first byte of the command is replaced by the 0xAA header
    fun decodeCommand(info: String): ByteArray? {
        if (info.length <= 3) {
            return null
        }
        val body =
            (1 until info.length / 2).map { k ->
                val hi = Character.digit(info[k * 2], 16).coerceAtLeast(0)
                val lo = Character.digit(info[k * 2 + 1], 16).coerceAtLeast(0)
                ((hi shl 4) or lo).toByte()
            }
        val payload = byteArrayOf(HEADER) + body.toByteArray()
        return payload.takeIf { it.size >= MIN_COMMAND_SIZE }
    }

    fun isSpeedCommand(payload: ByteArray) =
        payload[1] == 0x02.toByte() && payload[2] == 0x03.toByte()
}
