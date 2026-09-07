/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.haptics

import android.content.Context
import android.os.SystemProperties
import android.os.VibrationEffect
import android.os.Vibrator

object HapticUtils {
    private const val PROP_TOUCH_STYLE = "persist.sys.vibrator.touch_style"

    const val STYLE_CRISP = 0
    const val STYLE_GENTLE = 1

    fun getTouchStyle(): Int = SystemProperties.get(PROP_TOUCH_STYLE).toIntOrNull() ?: STYLE_CRISP

    fun setTouchStyle(context: Context, style: Int) {
        SystemProperties.set(PROP_TOUCH_STYLE, style.toString())

        // Live tactile feedback on selection
        val vibrator = context.getSystemService(Vibrator::class.java)
        if (vibrator?.hasVibrator() == true) {
            vibrator.vibrate(VibrationEffect.createPredefined(VibrationEffect.EFFECT_CLICK))
        }
    }
}
