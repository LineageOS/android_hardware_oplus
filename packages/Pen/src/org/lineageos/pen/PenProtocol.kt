/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import java.util.UUID

object PenProtocol {
    val SERVICE_UUID = UUID.fromString("0783b03e-8535-b5a0-7140-a304d2495cb7")
    val PRESS_CHAR_UUID = UUID.fromString("0783b03e-8535-b5a0-7140-a304d2495cbb")
    val SPEED_CHAR_UUID = UUID.fromString("0783b03e-8535-b5a0-7140-a304d2495cbc")
    val CCC_DESCRIPTOR_UUID = UUID.fromString("00002902-0000-1000-8000-00805f9b34fb")

    const val MTU = 100

    const val NODE_PENCIL_CONNECTED = 32
    const val NODE_BT_INFO = 169

    const val PRESSURE_CLEAR = "aa0101020000"

    // Every (KEEP_ALIVE_INTERVAL + 1)th pressure packet is acked on the speed characteristic
    const val KEEP_ALIVE_INTERVAL = 5
    val KEEP_ALIVE = "aa020000".hexToByteArray()

    fun isPressurePacket(value: ByteArray) =
        value.size >= 4 && value[2] == 0x01.toByte() && value[3] == 0x02.toByte()

    fun isSpeedCommand(payload: ByteArray) =
        payload[1] == 0x02.toByte() && payload[2] == 0x03.toByte()

    // The HAL leaves the first byte of a command blank, it has to be replaced with the 0xAA header
    fun decodeCommand(info: String) =
        runCatching { ("aa" + info.drop(2)).hexToByteArray() }.getOrNull()?.takeIf { it.size >= 4 }
}
