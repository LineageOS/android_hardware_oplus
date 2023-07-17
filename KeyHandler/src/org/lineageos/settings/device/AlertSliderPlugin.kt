/*
 * Copyright (C) 2019 CypherOS
 * Copyright (C) 2014-2020 Paranoid Android
 * Copyright (C) 2023 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
package org.lineageos.settings.device

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.Handler
import android.os.Looper
import android.os.Message
import android.view.View
import com.android.systemui.plugins.OverlayPlugin
import com.android.systemui.plugins.annotations.Requires

@Requires(target = OverlayPlugin::class, version = OverlayPlugin.VERSION)
class AlertSliderPlugin : OverlayPlugin {
    private lateinit var pluginContext: Context
    private lateinit var handler: NotificationHandler

    private data class NotificationInfo(
        val flip: Boolean,
        val position: Int,
        val mode: Int,
    )

    private val updateReceiver: BroadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context, intent: Intent) {
            when (intent.action) {
                KeyHandler.CHANGED_ACTION -> {
                    val display = intent.getIntExtra("display", SHOW_RIGHT)
                        .takeIf { it != DISABLED } ?: return
                    val ringer = intent.getIntExtra("mode", NONE)
                        .takeIf { it != NONE } ?: return

                    handler.obtainMessage(
                        MSG_DIALOG_UPDATE, NotificationInfo(
                            display == SHOW_LEFT,
                            intent.getIntExtra("position", KeyHandler.POSITION_BOTTOM),
                            ringer
                        )
                    ).sendToTarget()
                    handler.sendEmptyMessage(MSG_DIALOG_SHOW)
                }
            }
        }
    }

    override fun onCreate(context: Context, plugin: Context) {
        pluginContext = plugin
        handler = NotificationHandler(plugin)

        plugin.registerReceiver(updateReceiver, IntentFilter(KeyHandler.CHANGED_ACTION))
    }

    override fun onDestroy() {
        pluginContext.unregisterReceiver(updateReceiver)
    }

    override fun setup(statusBar: View, navBar: View) {}

    private inner class NotificationHandler(context: Context) : Handler(Looper.getMainLooper()) {
        private var dialog = AlertSliderDialog(context)
        private var showing = false
            set(value) {
                if (field != value) {
                    // Remove pending messages
                    removeMessages(MSG_DIALOG_SHOW)
                    removeMessages(MSG_DIALOG_DISMISS)

                    // Show/hide dialog
                    if (value) {
                        handleResetTimeout()
                        dialog.show()
                    } else {
                        dialog.dismiss()
                    }
                }

                field = value
            }

        override fun handleMessage(msg: Message) = when (msg.what) {
            MSG_DIALOG_SHOW -> handleShow()
            MSG_DIALOG_DISMISS -> handleDismiss()
            MSG_DIALOG_RESET -> handleResetTimeout()
            MSG_DIALOG_UPDATE -> handleUpdate(msg.obj as NotificationInfo)
            else -> {}
        }

        private fun handleShow() {
            showing = true
        }

        private fun handleDismiss() {
            showing = false
        }

        private fun handleResetTimeout() {
            removeMessages(MSG_DIALOG_DISMISS)
            sendMessageDelayed(
                handler.obtainMessage(MSG_DIALOG_DISMISS, MSG_DIALOG_RESET, 0), DIALOG_TIMEOUT
            )
        }

        private fun handleUpdate(info: NotificationInfo) {
            dialog.setState(info.position, info.mode, info.flip)
        }
    }

    companion object {
        private const val TAG = "AlertSliderPlugin"

        // Handler
        private const val MSG_DIALOG_SHOW = 1
        private const val MSG_DIALOG_DISMISS = 2
        private const val MSG_DIALOG_RESET = 3
        private const val MSG_DIALOG_UPDATE = 4
        private const val DIALOG_TIMEOUT = 2000L

        // Dialog
        private const val DISABLED = 0
        private const val SHOW_LEFT = 1
        private const val SHOW_RIGHT = 2

        // Ringer mode
        private const val NONE = -1
    }
}
