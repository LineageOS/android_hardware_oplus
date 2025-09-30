/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import android.view.InputDevice

enum class DeviceId(val productName: String) {
    // From OplusExSystemService/pencil_device_name
    ONEPLUS_PENCIL("OnePlus Pencil"),
    ONEPLUS_PENCIL_PRO("OnePlus Pencil Pro"),
    ONEPLUS_STYLO("OnePlus Stylo"),
    ONEPLUS_STYLO_2("OnePlus Stylo 2"),
    ONEPLUS_STYLO_R("OnePlus Stylo R"),
    OPPO_PENCIL("OPPO Pencil"),
    OPPO_PENCIL_2("OPPO Pencil 2"),
    OPPO_PENCIL_2_PRO("OPPO Pencil 2 Pro"),
    REALME_PENCIL("realme Pencil");

    companion object {
        fun fromInputDevice(device: InputDevice) = entries.firstOrNull {
            device.vendorId == 0x22D9 && device.name.startsWith("${it.productName} ")
        }
    }
}
