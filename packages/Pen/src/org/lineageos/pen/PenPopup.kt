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
import android.view.ViewGroup
import android.view.ViewTreeObserver
import android.view.WindowInsets
import android.view.WindowManager
import android.view.animation.PathInterpolator
import android.widget.ImageView
import android.widget.TextView

class PenPopup(context: Context, private val onDismissed: () -> Unit) {
    private val windowContext by lazy {
        val display =
            context.getSystemService(DisplayManager::class.java).getDisplay(Display.DEFAULT_DISPLAY)
        context.createDisplayContext(display).createWindowContext(WINDOW_TYPE, null)
    }
    private val windowManager by lazy { windowContext.getSystemService(WindowManager::class.java) }

    private val handler = Handler(context.mainLooper)
    private val dismissRunnable = Runnable { dismiss() }

    private var pill: ViewGroup? = null
    private var layoutParams: WindowManager.LayoutParams? = null

    var content: PenPopupContent? = null
        private set

    fun show(newContent: PenPopupContent) {
        handler.removeCallbacks(dismissRunnable)
        content = newContent

        val shownPill = pill
        if (shownPill != null) {
            bindInteraction(shownPill, newContent)
            crossfade(shownPill)
            animateIn(shownPill)
        } else {
            val newPill =
                LayoutInflater.from(windowContext).inflate(R.layout.pen_popup, null) as ViewGroup
            bindContent(newPill, newContent)
            bindInteraction(newPill, newContent)
            prepareEnter(newPill)
            val params = createLayoutParams(newPill, newContent)
            windowManager.addView(newPill, params)
            pill = newPill
            layoutParams = params
        }

        newContent.durationMs?.let { handler.postDelayed(dismissRunnable, it) }
    }

    fun dismiss() {
        handler.removeCallbacks(dismissRunnable)
        val shownPill = pill ?: return
        setTouchable(shownPill, false)
        shownPill
            .animate()
            .alpha(0f)
            .scaleX(COLLAPSED_SCALE_X)
            .scaleY(COLLAPSED_SCALE_Y)
            .translationY(enterOffset())
            .setDuration(EXIT_DURATION_MS)
            .setInterpolator(EMPHASIZED_ACCELERATE)
            .withEndAction { remove(shownPill) }
    }

    fun dismissNow() {
        handler.removeCallbacks(dismissRunnable)
        pill?.let { remove(it) }
    }

    private fun remove(view: ViewGroup) {
        view.animate().cancel()
        if (view.isAttachedToWindow) {
            windowManager.removeViewImmediate(view)
        }
        if (pill === view) {
            pill = null
            layoutParams = null
            content = null
            onDismissed()
        }
    }

    // Swap the contents while they're faded out, so a size change isn't seen as a jump
    private fun crossfade(view: ViewGroup) {
        for (i in 0 until view.childCount) {
            val child = view.getChildAt(i)
            child
                .animate()
                .alpha(0f)
                .setDuration(CONTENT_FADE_OUT_MS)
                .setInterpolator(EMPHASIZED_ACCELERATE)
                .withEndAction {
                    if (i == 0) {
                        content?.let { bindContent(view, it) }
                    }
                    child
                        .animate()
                        .alpha(1f)
                        .setDuration(CONTENT_FADE_IN_MS)
                        .setInterpolator(EMPHASIZED_DECELERATE)
                }
        }
    }

    private fun bindContent(view: ViewGroup, content: PenPopupContent) {
        val res = windowContext.resources
        val theme = windowContext.theme
        fun color(id: Int) = ColorStateList.valueOf(res.getColor(id, theme))

        val container =
            if (content.isError) R.color.pen_popup_error_container
            else R.color.pen_popup_primary_container
        val onContainer =
            if (content.isError) R.color.pen_popup_on_error_container
            else R.color.pen_popup_on_primary_container
        val accent = if (content.isError) R.color.pen_popup_error else R.color.pen_popup_primary

        view.requireViewById<View>(R.id.icon_container).backgroundTintList = color(container)
        view.requireViewById<ImageView>(R.id.icon).imageTintList = color(onContainer)
        view.requireViewById<TextView>(R.id.title).text = content.title
        view.requireViewById<TextView>(R.id.subtitle).text = content.subtitle
        view.requireViewById<ImageView>(R.id.bolt).apply {
            visibility =
                if (content.isCharging && content.levelText != null) View.VISIBLE else View.GONE
            imageTintList = color(accent)
        }
        view.requireViewById<TextView>(R.id.level).apply {
            visibility = if (content.levelText != null) View.VISIBLE else View.GONE
            text = content.levelText
            setTextColor(res.getColor(accent, theme))
        }
        view.accessibilityPaneTitle =
            listOfNotNull(content.title, content.subtitle, content.levelText).joinToString()
    }

    private fun bindInteraction(view: View, content: PenPopupContent) {
        val onClick = content.onClick
        if (onClick != null) {
            view.setOnClickListener { onClick() }
        } else {
            view.setOnClickListener(null)
            view.isClickable = false
        }
        setTouchable(view, onClick != null)
    }

    private fun setTouchable(view: View, isTouchable: Boolean) {
        val params = layoutParams ?: return
        val flags =
            if (isTouchable) params.flags and WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE.inv()
            else params.flags or WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE
        if (flags != params.flags && view.isAttachedToWindow) {
            params.flags = flags
            windowManager.updateViewLayout(view, params)
        }
    }

    private fun prepareEnter(view: View) {
        view.alpha = 0f
        view.scaleX = COLLAPSED_SCALE_X
        view.scaleY = COLLAPSED_SCALE_Y
        view.translationY = enterOffset()
        view.viewTreeObserver.addOnPreDrawListener(
            object : ViewTreeObserver.OnPreDrawListener {
                override fun onPreDraw(): Boolean {
                    view.viewTreeObserver.removeOnPreDrawListener(this)
                    animateIn(view)
                    return true
                }
            }
        )
    }

    private fun animateIn(view: View) {
        view.pivotX = view.width / 2f
        view.pivotY = 0f
        view
            .animate()
            .alpha(1f)
            .scaleX(1f)
            .scaleY(1f)
            .translationY(0f)
            .setDuration(ENTER_DURATION_MS)
            .setInterpolator(EMPHASIZED_DECELERATE)
    }

    private fun enterOffset() =
        windowContext.resources.getDimension(R.dimen.pen_popup_enter_offset)

    private fun createLayoutParams(view: View, content: PenPopupContent): WindowManager.LayoutParams {
        val statusBarHeight =
            windowManager.currentWindowMetrics.windowInsets
                .getInsets(WindowInsets.Type.statusBars())
                .top
        val res = windowContext.resources
        val marginTop = res.getDimensionPixelSize(R.dimen.pen_popup_margin_top)

        var flags =
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE or
                WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN
        if (content.onClick == null) {
            flags = flags or WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE
        }

        return WindowManager.LayoutParams(
                WindowManager.LayoutParams.WRAP_CONTENT,
                res.getDimensionPixelSize(R.dimen.pen_popup_height),
                WINDOW_TYPE,
                flags,
                PixelFormat.TRANSLUCENT,
            )
            .apply {
                title = TAG
                gravity = Gravity.TOP or Gravity.CENTER_HORIZONTAL
                y = statusBarHeight + marginTop
                layoutInDisplayCutoutMode =
                    WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_ALWAYS
                fitInsetsTypes = 0
                // Let the shadow draw outside the window without growing its touchable area
                setSurfaceInsets(view, true /* manual */, false /* preservePrevious */)
            }
    }

    companion object {
        private const val TAG = "OplusPenPopup"

        private const val WINDOW_TYPE = WindowManager.LayoutParams.TYPE_STATUS_BAR_SUB_PANEL

        private const val ENTER_DURATION_MS = 450L
        private const val EXIT_DURATION_MS = 200L
        private const val CONTENT_FADE_OUT_MS = 100L
        private const val CONTENT_FADE_IN_MS = 200L

        private const val COLLAPSED_SCALE_X = 0.4f
        private const val COLLAPSED_SCALE_Y = 0.6f

        private val EMPHASIZED_DECELERATE = PathInterpolator(0.05f, 0.7f, 0.1f, 1f)
        private val EMPHASIZED_ACCELERATE = PathInterpolator(0.3f, 0f, 0.8f, 0.15f)
    }
}
