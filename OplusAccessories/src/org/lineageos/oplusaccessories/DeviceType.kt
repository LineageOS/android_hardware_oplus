/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.oplusaccessories

import android.view.InputDevice

enum class DeviceType(val icon: Int, val title: Int) {
    KEYBOARD(R.drawable.ic_keyboard, R.string.keyboard_attached),
    PEN(R.drawable.ic_stylus, R.string.pen_attached),
}
