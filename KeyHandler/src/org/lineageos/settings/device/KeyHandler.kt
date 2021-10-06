/*
 * Copyright (C) 2021 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device

import android.content.Context
import android.media.AudioManager
import android.os.VibrationEffect
import android.os.Vibrator
import android.view.KeyEvent
import com.android.internal.os.DeviceKeyHandler

class KeyHandler(context: Context) : DeviceKeyHandler {
    private val audioManager = context.getSystemService(AudioManager::class.java)
    private val vibrator = context.getSystemService(Vibrator::class.java)
    private val packageContext = context.createPackageContext(
        KeyHandler::class.java.getPackage()!!.name, 0
    )
    private val sharedPreferences
        get() = packageContext.getSharedPreferences(
            packageContext.packageName + "_preferences",
            Context.MODE_PRIVATE or Context.MODE_MULTI_PROCESS
        )

    override fun handleKeyEvent(event: KeyEvent): KeyEvent {
        if (event.action == KeyEvent.ACTION_DOWN) {
            when (event.scanCode) {
                POSITION_TOP -> {
                    val mode = sharedPreferences.getString(ALERT_SLIDER_TOP_KEY, "0")!!.toInt()
                    audioManager.setRingerModeInternal(mode)
                    vibrateIfNeeded(mode)
                }
                POSITION_MIDDLE -> {
                    val mode = sharedPreferences.getString(ALERT_SLIDER_MIDDLE_KEY, "1")!!.toInt()
                    audioManager.setRingerModeInternal(mode)
                    vibrateIfNeeded(mode)
                }
                POSITION_BOTTOM -> {
                    val mode = sharedPreferences.getString(ALERT_SLIDER_BOTTOM_KEY, "2")!!.toInt()
                    audioManager.setRingerModeInternal(mode)
                    vibrateIfNeeded(mode)
                }
            }
        }
        return event
    }

    private fun vibrateIfNeeded(mode: Int) {
        when (mode) {
            AudioManager.RINGER_MODE_VIBRATE -> vibrator.vibrate(MODE_VIBRATION_EFFECT)
            AudioManager.RINGER_MODE_NORMAL -> vibrator.vibrate(MODE_NORMAL_EFFECT)
        }
    }

    companion object {
        private const val TAG = "KeyHandler"

        // Slider key codes
        private const val POSITION_TOP = 601
        private const val POSITION_MIDDLE = 602
        private const val POSITION_BOTTOM = 603

        // Preference keys
        private const val ALERT_SLIDER_TOP_KEY = "config_top_position"
        private const val ALERT_SLIDER_MIDDLE_KEY = "config_middle_position"
        private const val ALERT_SLIDER_BOTTOM_KEY = "config_bottom_position"

        // Vibration effects
        private val MODE_NORMAL_EFFECT = VibrationEffect.get(VibrationEffect.EFFECT_HEAVY_CLICK)
        private val MODE_VIBRATION_EFFECT = VibrationEffect.get(VibrationEffect.EFFECT_DOUBLE_CLICK)
    }
}
