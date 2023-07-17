/*
 * Copyright (C) 2019 CypherOS
 * Copyright (C) 2014-2020 Paranoid Android
 * Copyright (C) 2023 The Evervolv Project
 * Copyright (C) 2023 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device

import android.app.Dialog
import android.content.Context
import android.graphics.Color
import android.graphics.PixelFormat
import android.graphics.drawable.ColorDrawable
import android.graphics.drawable.Drawable
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

    private val frameView: ViewGroup
    private val iconView: ImageView
    private val textView: TextView

    init {
        window!!.requestFeature(Window.FEATURE_NO_TITLE)
        window!!.setBackgroundDrawable(ColorDrawable(Color.TRANSPARENT))
        window!!.clearFlags(
            WindowManager.LayoutParams.FLAG_DIM_BEHIND
        )
        window!!.addFlags(
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
                    or WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL
                    or WindowManager.LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH
                    or WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED
        )
        window.addPrivateFlags(WindowManager.LayoutParams.PRIVATE_FLAG_TRUSTED_OVERLAY)
        window!!.setType(WindowManager.LayoutParams.TYPE_VOLUME_OVERLAY)

        val lp = window!!.attributes
        lp.format = PixelFormat.TRANSLUCENT
        lp.title = TAG
        window!!.attributes = lp

        window!!.setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_NOTHING)
        setCanceledOnTouchOutside(false)

        setContentView(R.layout.alert_slider_dialog)

        frameView = findViewById(R.id.alert_slider_view)
        iconView = findViewById(R.id.alert_slider_icon)
        textView = findViewById(R.id.alert_slider_text)
    }

    fun setState(position: Int, ringerMode: Int, flip: Boolean) {
        val lp = window!!.attributes
        lp.gravity = if (flip) {
            Gravity.TOP or Gravity.LEFT
        } else {
            Gravity.TOP or Gravity.RIGHT
        }
        lp.x = context.resources.displayMetrics.widthPixels / 100
        val step = context.resources.displayMetrics.heightPixels / 10
        lp.y = context.resources.getDimension(com.android.internal.R.dimen.status_bar_height).toInt()
        lp.y += step * position
        window!!.attributes = lp

        var background = when (position) {
            KeyHandler.POSITION_TOP -> {
                if (flip) R.drawable.alert_slider_top_flip
                else R.drawable.alert_slider_top
            }
            KeyHandler.POSITION_MIDDLE -> R.drawable.alert_slider_middle
            else -> {
                if (flip) R.drawable.alert_slider_bottom_flip
                else R.drawable.alert_slider_bottom
            }
        }

        frameView.setBackgroundResource(background)

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
