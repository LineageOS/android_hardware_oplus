/*
 * SPDX-FileCopyrightText: 2019 CypherOS
 * SPDX-FileCopyrightText: 2014-2020 Paranoid Android
 * SPDX-FileCopyrightText: 2023-2026 The LineageOS Project
 * SPDX-FileCopyrightText: 2023 Yet Another AOSP Project
 * SPDX-License-Identifier: Apache-2.0
 */
package org.lineageos.settings.device

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.hardware.display.AmbientDisplayConfiguration
import android.os.Handler
import android.os.Looper
import android.os.Message
import android.os.UserHandle
import android.view.View
import com.android.systemui.plugins.OverlayPlugin
import com.android.systemui.plugins.annotations.Requires

@Requires(target = OverlayPlugin::class, version = OverlayPlugin.VERSION)
class AlertSliderPlugin : OverlayPlugin {
    private lateinit var ambientConfig: AmbientDisplayConfiguration
    private lateinit var pluginContext: Context
    private lateinit var handler: NotificationHandler
    private val dialogLock = Any()

    private data class NotificationInfo(val position: Int, val mode: Int)

    private val updateReceiver: BroadcastReceiver =
        object : BroadcastReceiver() {
            override fun onReceive(context: Context, intent: Intent) {
                when (intent.action) {
                    Intent.ACTION_CONFIGURATION_CHANGED -> {
                        synchronized(dialogLock) { handler.sendEmptyMessage(MSG_DIALOG_RECREATE) }
                    }
                    KeyHandler.CHANGED_ACTION -> {
                        synchronized(dialogLock) {
                            val ringer =
                                intent.getIntExtra("mode", NONE).takeIf { it != NONE } ?: return

                            handler
                                .obtainMessage(
                                    MSG_DIALOG_UPDATE,
                                    NotificationInfo(
                                        intent.getIntExtra("position", KeyHandler.POSITION_BOTTOM),
                                        ringer,
                                    ),
                                )
                                .sendToTarget()
                            handler.sendEmptyMessage(MSG_DIALOG_SHOW)
                        }
                    }
                }
            }
        }

    override fun onCreate(context: Context, plugin: Context) {
        ambientConfig = AmbientDisplayConfiguration(context)
        pluginContext = plugin
        handler = NotificationHandler(plugin, context)

        val filter =
            IntentFilter().apply {
                addAction(Intent.ACTION_CONFIGURATION_CHANGED)
                addAction(KeyHandler.CHANGED_ACTION)
            }
        plugin.registerReceiver(updateReceiver, filter)
    }

    override fun onDestroy() {
        pluginContext.unregisterReceiver(updateReceiver)
    }

    override fun setup(statusBar: View?, navBar: View?) {}

    private inner class NotificationHandler(
        var context: Context,
        private var sysuiContext: Context,
    ) : Handler(Looper.getMainLooper()) {
        private var dialog = AlertSliderDialog(context, sysuiContext)
        private var currDensity = context.resources.configuration.densityDpi
        private var currRotation = context.display.rotation
        private var currSmallestWidth = context.resources.configuration.smallestScreenWidthDp
        private var currUIMode = context.resources.configuration.uiMode
        private var lastInfo: NotificationInfo? = null
        private var showing = false
            set(value) {
                synchronized(dialogLock) {
                    if (field != value) {
                        // Remove pending messages
                        removeMessages(MSG_DIALOG_SHOW)
                        removeMessages(MSG_DIALOG_DISMISS)
                        removeMessages(MSG_DIALOG_RESET)

                        // Show/hide dialog
                        if (value) {
                            handleResetTimeout()
                            launchDozePulse()
                            dialog.show()
                        } else {
                            dialog.dismiss()
                        }
                    }

                    field = value
                }
            }

        override fun handleMessage(msg: Message) =
            when (msg.what) {
                MSG_DIALOG_SHOW -> handleShow()
                MSG_DIALOG_DISMISS -> handleDismiss()
                MSG_DIALOG_RESET -> handleResetTimeout()
                MSG_DIALOG_UPDATE -> handleUpdate(msg.obj as NotificationInfo)
                MSG_DIALOG_RECREATE -> handleRecreate()
                else -> {}
            }

        private fun handleShow() {
            showing = true
        }

        private fun handleDismiss() {
            showing = false
        }

        private fun handleResetTimeout() {
            synchronized(dialogLock) {
                removeMessages(MSG_DIALOG_DISMISS)
                sendMessageDelayed(
                    handler.obtainMessage(MSG_DIALOG_DISMISS, MSG_DIALOG_RESET, 0),
                    DIALOG_TIMEOUT,
                )
            }
        }

        private fun handleUpdate(info: NotificationInfo) {
            synchronized(dialogLock) {
                lastInfo = info
                handleResetTimeout()
                launchDozePulse()
                dialog.setState(info.position, info.mode)
            }
        }

        private fun handleRecreate() {
            val config = context.resources.configuration
            val density = config.densityDpi
            val rotation = context.display.rotation
            val smallestWidth = config.smallestScreenWidthDp
            val uiMode = config.uiMode

            val densityChanged = density != currDensity
            val layoutChanged = smallestWidth != currSmallestWidth
            val rotationChanged = rotation != currRotation
            val themeChanged = uiMode != currUIMode
            if (densityChanged || layoutChanged || rotationChanged || themeChanged) {
                val wasShowing = showing

                showing = false
                dialog = AlertSliderDialog(context, sysuiContext)
                lastInfo?.let { dialog.setState(it.position, it.mode) }

                if (wasShowing) {
                    showing = true
                }

                currDensity = density
                currRotation = rotation
                currSmallestWidth = smallestWidth
                currUIMode = uiMode
            }
        }

        private fun launchDozePulse() {
            if (ambientConfig.pulseOnNotificationEnabled(UserHandle.USER_CURRENT)) {
                context.sendBroadcastAsUser(Intent(DOZE_INTENT), UserHandle.CURRENT)
            }
        }
    }

    companion object {
        private const val TAG = "AlertSliderPlugin"

        // Intent
        private const val DOZE_INTENT = "com.android.systemui.doze.pulse"

        // Handler
        private const val MSG_DIALOG_SHOW = 1
        private const val MSG_DIALOG_DISMISS = 2
        private const val MSG_DIALOG_RESET = 3
        private const val MSG_DIALOG_UPDATE = 4
        private const val MSG_DIALOG_RECREATE = 5
        private const val DIALOG_TIMEOUT = 3000L

        // Dialog
        private const val DISABLED = 0
        private const val SHOW_LEFT = 1
        private const val SHOW_RIGHT = 2

        // Ringer mode
        private const val NONE = -1
    }
}
