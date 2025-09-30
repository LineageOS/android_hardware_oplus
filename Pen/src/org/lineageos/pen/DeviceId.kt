/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import android.view.InputDevice

enum class DeviceId(val vendorId: Int, val productId: Int) {
    ONEPLUS_STYLO_2(0x22D9, 0x386A);

    companion object {
        fun fromInputDevice(device: InputDevice) = entries.firstOrNull {
            it.productId == device.productId && it.vendorId == device.vendorId
        }
    }
}
