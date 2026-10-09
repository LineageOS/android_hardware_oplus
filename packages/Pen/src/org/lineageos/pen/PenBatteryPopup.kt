/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import android.content.Context
import android.content.res.ColorStateList
import android.graphics.PixelFormat
import android.hardware.display.DisplayManager
import android.os.Handler
import android.view.Display
import android.view.Gravity
import android.view.LayoutInflater
import android.view.View
import android.view.ViewTreeObserver
import android.view.WindowInsets
import android.view.WindowManager
import android.view.animation.PathInterpolator
import android.widget.ImageView
import android.widget.TextView
import java.text.NumberFormat

class PenBatteryPopup(context: Context) {
    private val windowContext by lazy {
        val display =
            context.getSystemService(DisplayManager::class.java).getDisplay(Display.DEFAULT_DISPLAY)
        context.createDisplayContext(display).createWindowContext(WINDOW_TYPE, null)
    }
    private val windowManager by lazy { windowContext.getSystemService(WindowManager::class.java) }

    private val handler = Handler(context.mainLooper)
    private val dismissRunnable = Runnable { dismiss() }

    private var root: View? = null

    fun show(name: String, level: Int, isLowBattery: Boolean) {
        handler.removeCallbacks(dismissRunnable)

        val shownRoot = root
        if (shownRoot != null) {
            bind(shownRoot, name, level, isLowBattery)
            animateIn(shownRoot.requireViewById(R.id.pill))
        } else {
            val newRoot =
                LayoutInflater.from(windowContext).inflate(R.layout.pen_battery_popup, null)
            bind(newRoot, name, level, isLowBattery)
            prepareEnter(newRoot.requireViewById(R.id.pill))
            windowManager.addView(newRoot, createLayoutParams())
            root = newRoot
        }

        handler.postDelayed(
            dismissRunnable,
            if (isLowBattery) LOW_BATTERY_DISPLAY_DURATION_MS else DISPLAY_DURATION_MS,
        )
    }

    fun dismissNow() {
        handler.removeCallbacks(dismissRunnable)
        root?.let { removeRoot(it) }
    }

    private fun dismiss() {
        val shownRoot = root ?: return
        val pill = shownRoot.requireViewById<View>(R.id.pill)
        pill
            .animate()
            .alpha(0f)
            .scaleX(COLLAPSED_SCALE_X)
            .scaleY(COLLAPSED_SCALE_Y)
            .translationY(enterOffset())
            .setDuration(EXIT_DURATION_MS)
            .setInterpolator(EMPHASIZED_ACCELERATE)
            .withEndAction { removeRoot(shownRoot) }
    }

    private fun removeRoot(view: View) {
        view.requireViewById<View>(R.id.pill).animate().cancel()
        if (view.isAttachedToWindow) {
            windowManager.removeViewImmediate(view)
        }
        if (root === view) {
            root = null
        }
    }

    private fun bind(view: View, name: String, level: Int, isLowBattery: Boolean) {
        val res = windowContext.resources
        val theme = windowContext.theme

        val subtitle =
            when {
                isLowBattery -> res.getString(R.string.pen_battery_low)
                level == FULL_LEVEL -> res.getString(R.string.pen_fully_charged)
                else -> res.getString(R.string.pen_charging)
            }
        val levelText =
            if (level in 0..FULL_LEVEL) {
                NumberFormat.getPercentInstance().format(level / FULL_LEVEL.toDouble())
            } else {
                null
            }

        val containerColor =
            if (isLowBattery) R.color.pen_popup_error_container
            else R.color.pen_popup_primary_container
        val onContainerColor =
            if (isLowBattery) R.color.pen_popup_on_error_container
            else R.color.pen_popup_on_primary_container
        val accentColor = if (isLowBattery) R.color.pen_popup_error else R.color.pen_popup_primary

        view.requireViewById<View>(R.id.icon_container).backgroundTintList =
            ColorStateList.valueOf(res.getColor(containerColor, theme))
        view.requireViewById<ImageView>(R.id.icon).imageTintList =
            ColorStateList.valueOf(res.getColor(onContainerColor, theme))
        view.requireViewById<TextView>(R.id.title).text = name
        view.requireViewById<TextView>(R.id.subtitle).text = subtitle
        view.requireViewById<ImageView>(R.id.bolt).apply {
            visibility = if (isLowBattery || levelText == null) View.GONE else View.VISIBLE
            imageTintList = ColorStateList.valueOf(res.getColor(accentColor, theme))
        }
        view.requireViewById<TextView>(R.id.level).apply {
            visibility = if (levelText == null) View.GONE else View.VISIBLE
            text = levelText
            setTextColor(res.getColor(accentColor, theme))
        }
        view.requireViewById<View>(R.id.pill).accessibilityPaneTitle =
            listOfNotNull(name, subtitle, levelText).joinToString()
    }

    private fun prepareEnter(pill: View) {
        pill.alpha = 0f
        pill.scaleX = COLLAPSED_SCALE_X
        pill.scaleY = COLLAPSED_SCALE_Y
        pill.translationY = enterOffset()
        pill.viewTreeObserver.addOnPreDrawListener(
            object : ViewTreeObserver.OnPreDrawListener {
                override fun onPreDraw(): Boolean {
                    pill.viewTreeObserver.removeOnPreDrawListener(this)
                    animateIn(pill)
                    return true
                }
            }
        )
    }

    private fun animateIn(pill: View) {
        pill.pivotX = pill.width / 2f
        pill.pivotY = 0f
        pill
            .animate()
            .alpha(1f)
            .scaleX(1f)
            .scaleY(1f)
            .translationY(0f)
            .setDuration(ENTER_DURATION_MS)
            .setInterpolator(EMPHASIZED_DECELERATE)
    }

    private fun enterOffset() = windowContext.resources.getDimension(R.dimen.pen_popup_enter_offset)

    private fun createLayoutParams(): WindowManager.LayoutParams {
        val res = windowContext.resources
        val statusBarHeight =
            windowManager.currentWindowMetrics.windowInsets
                .getInsets(WindowInsets.Type.statusBars())
                .top
        val shadowInset = res.getDimensionPixelSize(R.dimen.pen_popup_shadow_inset)
        val marginTop = res.getDimensionPixelSize(R.dimen.pen_popup_margin_top)

        return WindowManager.LayoutParams(
                WindowManager.LayoutParams.WRAP_CONTENT,
                WindowManager.LayoutParams.WRAP_CONTENT,
                WINDOW_TYPE,
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE or
                    WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE or
                    WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
                PixelFormat.TRANSLUCENT,
            )
            .apply {
                title = TAG
                gravity = Gravity.TOP or Gravity.CENTER_HORIZONTAL
                y = statusBarHeight + marginTop - shadowInset
                layoutInDisplayCutoutMode =
                    WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_ALWAYS
                fitInsetsTypes = 0
            }
    }

    companion object {
        private const val TAG = "OplusPenBatteryPopup"

        private const val WINDOW_TYPE = WindowManager.LayoutParams.TYPE_SECURE_SYSTEM_OVERLAY

        private const val FULL_LEVEL = 100

        private const val DISPLAY_DURATION_MS = 2500L
        private const val LOW_BATTERY_DISPLAY_DURATION_MS = 4000L
        private const val ENTER_DURATION_MS = 450L
        private const val EXIT_DURATION_MS = 200L

        private const val COLLAPSED_SCALE_X = 0.4f
        private const val COLLAPSED_SCALE_Y = 0.6f

        private val EMPHASIZED_DECELERATE = PathInterpolator(0.05f, 0.7f, 0.1f, 1f)
        private val EMPHASIZED_ACCELERATE = PathInterpolator(0.3f, 0f, 0.8f, 0.15f)
    }
}
