/*
 * Copyright 2019 CypherOS
 * Copyright 2014-2020 Paranoid Android
 * Copyright 2023 The Evervolv Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the
 * License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
package org.lineageos.settings.device

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.Handler
import android.os.Looper
import android.os.Message
import android.util.Log
import android.view.View
import com.android.systemui.plugins.OverlayPlugin
import com.android.systemui.plugins.annotations.Requires

@Requires(target = OverlayPlugin::class, version = OverlayPlugin.VERSION)
class AlertSliderPlugin : OverlayPlugin {

    private var pluginContext: Context? = null
    private var handler: NotificationHandler? = null

    private data class NotificationInfo(
        val flip: Boolean,
        val position: Int,
        val mode: Int,
    )

    private val updateReceiver: BroadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context, intent: Intent) {
            if (intent.action == KeyHandler.CHANGED_ACTION) {
                var display = intent.getIntExtra("display", SHOW_RIGHT) 
                var pos = intent.getIntExtra("position", KeyHandler.POSITION_BOTTOM)
                var ringer = intent.getIntExtra("mode", NONE)
                if (DEBUG) {
                    Log.d(TAG, "display: $display")
                    if (display != 0) {
                        Log.d(TAG, "position: $pos")
                        Log.d(TAG, "mode: $ringer")
                    }
                }

                if (display == DISABLED || ringer == NONE) {
                    return
                }

                var info: NotificationInfo = NotificationInfo(
                    flip = when(display) {
                        SHOW_LEFT -> true
                        else -> false
                    },
                    position = pos,
                    mode = ringer
                )

                handler!!.obtainMessage(MSG_DIALOG_UPDATE, info).sendToTarget()
                if (DEBUG) Log.d(TAG, "sending show message")
                handler!!.sendEmptyMessage(MSG_DIALOG_SHOW)
            }
        }
    }

    override fun onCreate(context: Context, plugin: Context) {
        pluginContext = plugin
        handler = NotificationHandler(plugin)
        val filter = IntentFilter(KeyHandler.CHANGED_ACTION)
        plugin.registerReceiver(updateReceiver, filter)
    }

    override fun onDestroy() {
        pluginContext!!.unregisterReceiver(updateReceiver)
    }

    override fun setup(statusBar: View, navBar: View) { }

    private inner class NotificationHandler(context: Context) : Handler(Looper.getMainLooper()) {
        private var dialog: AlertSliderDialog? = null
        private var showing: Boolean = false

        init {
            dialog = AlertSliderDialog(context)
        }

        override fun handleMessage(msg: Message) {
            when (msg.what) {
                MSG_DIALOG_SHOW -> {
                    handleShow()
                    return
                }

                MSG_DIALOG_DISMISS -> {
                    handleDismiss()
                    return
                }

                MSG_DIALOG_RESET -> {
                    handleResetTimeout()
                    return
                }

                MSG_DIALOG_UPDATE -> {
                    val info = msg.obj as NotificationInfo
                    handleUpdate(info)
                    return
                }

                else -> return
            }
        }

        private fun clearMessages() {
            removeMessages(MSG_DIALOG_SHOW)
            removeMessages(MSG_DIALOG_DISMISS)
        }

        private fun handleShow() {
            clearMessages()
            handleResetTimeout()
            if (!showing) {
                showing = true
                dialog!!.show()
            }
        }

        private fun handleDismiss() {
            clearMessages()
            if (showing) {
                showing = false
                dialog!!.dismiss()
            }
        }

        private fun handleResetTimeout() {
            removeMessages(MSG_DIALOG_DISMISS)
            sendMessageDelayed(
                handler!!.obtainMessage(
                    MSG_DIALOG_DISMISS,
                    MSG_DIALOG_RESET,
                    0
                ), DIALOG_TIMEOUT.toLong()
            )
        }

        private fun handleUpdate(info: NotificationInfo) {
            dialog!!.setState(info.position, info.mode, info.flip)
        }
    }

    companion object {
        private const val TAG = "AlertSliderPlugin"
        private const val DEBUG = true

        // Handler
        private const val MSG_DIALOG_SHOW = 1
        private const val MSG_DIALOG_DISMISS = 2
        private const val MSG_DIALOG_RESET = 3
        private const val MSG_DIALOG_UPDATE = 4
        private const val DIALOG_TIMEOUT = 2000

        // Dialog
        private const val DISABLED = 0
        private const val SHOW_LEFT = 1
        private const val SHOW_RIGHT = 2

        // Ringer mode
        private const val NONE = -1
    }
}
