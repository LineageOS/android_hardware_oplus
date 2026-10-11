/*
 * SPDX-FileCopyrightText: 2025-2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher

import android.content.BroadcastReceiver
import android.content.ComponentName
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.os.UserHandle
import android.util.Log
import java.util.concurrent.atomic.AtomicBoolean
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.asExecutor

class BootCompletedReceiver : BroadcastReceiver() {
    override fun onReceive(context: Context, intent: Intent) {
        if (
            intent.action != Intent.ACTION_LOCKED_BOOT_COMPLETED &&
                intent.action != Intent.ACTION_BOOT_COMPLETED
        )
            return
        if (context.userId != UserHandle.USER_SYSTEM) return
        Log.d(TAG, "Received ${intent.action}")

        val hasNonRemovableEuicc =
            context.resources
                .getIntArray(com.android.internal.R.array.non_removable_euicc_slots)
                .isNotEmpty()

        setComponentEnabled(context, EsimSettingsActivity::class.java.name, hasNonRemovableEuicc)
        // Keep normal boot as a fallback, without repeating successful locked-boot recovery.
        if (!hasNonRemovableEuicc || !recoveryStarted.compareAndSet(false, true)) return

        val pendingResult = goAsync()
        Dispatchers.IO.asExecutor().execute {
            try {
                EsimController(context.applicationContext).restoreEsimState { success ->
                    Log.i(TAG, "Boot eSIM recovery completed: success=$success")
                    if (!success) recoveryStarted.set(false)
                    pendingResult.finish()
                }
            } catch (e: Exception) {
                Log.e(TAG, "Boot eSIM recovery failed", e)
                recoveryStarted.set(false)
                pendingResult.finish()
            }
        }
    }

    private fun setComponentEnabled(context: Context, component: String, enabled: Boolean) {
        val name = ComponentName(context, component)
        val pm = context.packageManager
        val newState =
            if (enabled) {
                PackageManager.COMPONENT_ENABLED_STATE_ENABLED
            } else {
                PackageManager.COMPONENT_ENABLED_STATE_DISABLED
            }

        if (pm.getComponentEnabledSetting(name) != newState) {
            pm.setComponentEnabledSetting(name, newState, PackageManager.DONT_KILL_APP)
        }
    }

    companion object {
        private const val TAG = "OplusEsimBootReceiver"
        private val recoveryStarted = AtomicBoolean(false)
    }
}
