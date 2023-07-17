/*
 * Copyright (C) 2019 CypherOS
 * Copyright (C) 2014-2020 Paranoid Android
 * Copyright (C) 2023 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device

import android.app.Dialog
import android.content.Context
import android.graphics.Color
import android.graphics.PixelFormat
import android.graphics.drawable.ColorDrawable
import android.media.AudioManager
import android.view.Gravity
import android.view.ViewGroup
import android.view.Window
import android.view.WindowManager
import android.widget.ImageView
import android.widget.TextView

import org.lineageos.settings.device.R

/**
 * View with some logging to show that its being run.
 */
class AlertSliderDialog(context: Context) : Dialog(context, R.style.alert_slider_theme) {
    private val frameView by lazy { findViewById<ViewGroup>(R.id.alert_slider_view) }
    private val iconView by lazy { findViewById<ImageView>(R.id.alert_slider_icon) }
    private val textView by lazy { findViewById<TextView>(R.id.alert_slider_text) }

    init {
        window!!.requestFeature(Window.FEATURE_NO_TITLE)
        window.setBackgroundDrawable(ColorDrawable(Color.TRANSPARENT))
        window.clearFlags(WindowManager.LayoutParams.FLAG_DIM_BEHIND)
        window.addFlags(
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
                    or WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL
                    or WindowManager.LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH
                    or WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED
        )
        window.addPrivateFlags(WindowManager.LayoutParams.PRIVATE_FLAG_TRUSTED_OVERLAY)
        window.setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_NOTHING)
        window.setType(WindowManager.LayoutParams.TYPE_VOLUME_OVERLAY)
        window.attributes = window.attributes.apply {
            format = PixelFormat.TRANSLUCENT
            title = TAG
        }

        setCanceledOnTouchOutside(false)
        setContentView(R.layout.alert_slider_dialog)
    }

    fun setState(position: Int, ringerMode: Int, flip: Boolean) {
        window!!.attributes = window.attributes.apply {
            gravity = if (flip) {
                Gravity.TOP or Gravity.LEFT
            } else {
                Gravity.TOP or Gravity.RIGHT
            }

            x = context.resources.displayMetrics.widthPixels / 100
            y = context.resources.getDimension(
                com.android.internal.R.dimen.status_bar_height
            ).toInt()

            val step = context.resources.displayMetrics.heightPixels / 10
            y += step * position
        }

        frameView.setBackgroundResource(when (position) {
            KeyHandler.POSITION_TOP -> if (flip) {
                R.drawable.alert_slider_top_flip
            } else {
                R.drawable.alert_slider_top
            }
            KeyHandler.POSITION_MIDDLE -> R.drawable.alert_slider_middle
            else -> if (flip) {
                R.drawable.alert_slider_bottom_flip
            } else {
                R.drawable.alert_slider_bottom
            }
        })

        iconView.setImageResource(when (ringerMode) {
            AudioManager.RINGER_MODE_SILENT -> R.drawable.ic_volume_ringer_mute
            AudioManager.RINGER_MODE_VIBRATE -> R.drawable.ic_volume_ringer_vibrate
            AudioManager.RINGER_MODE_NORMAL -> R.drawable.ic_volume_ringer
            KeyHandler.ZEN_PRIORITY_ONLY -> R.drawable.ic_notifications_alert
            KeyHandler.ZEN_TOTAL_SILENCE -> R.drawable.ic_notifications_silence
            KeyHandler.ZEN_ALARMS_ONLY -> R.drawable.ic_alarm
            else -> R.drawable.ic_info
        })

        textView.setText(when (ringerMode) {
            AudioManager.RINGER_MODE_SILENT -> R.string.alert_slider_mode_silent
            AudioManager.RINGER_MODE_VIBRATE -> R.string.alert_slider_mode_vibration
            AudioManager.RINGER_MODE_NORMAL -> R.string.alert_slider_mode_normal
            KeyHandler.ZEN_PRIORITY_ONLY -> R.string.alert_slider_mode_dnd_priority_only
            KeyHandler.ZEN_TOTAL_SILENCE -> R.string.alert_slider_mode_dnd_total_silence
            KeyHandler.ZEN_ALARMS_ONLY -> R.string.alert_slider_mode_dnd_alarms_only
            else -> R.string.alert_slider_mode_none
        })
    }

    companion object {
        private const val TAG = "AlertSliderDialog"
    }
}
