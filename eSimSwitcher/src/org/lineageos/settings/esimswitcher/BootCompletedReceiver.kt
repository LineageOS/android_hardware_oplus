/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher

import android.content.BroadcastReceiver
import android.content.ComponentName
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.util.Log

class BootCompletedReceiver : BroadcastReceiver() {
    override fun onReceive(context: Context, intent: Intent) {
        Log.d(TAG, "Received boot completed intent")

        val hasNonRemovableEuicc =
            context.resources
                .getIntArray(com.android.internal.R.array.non_removable_euicc_slots)
                .isNotEmpty()

        setComponentEnabled(context, EsimSettingsActivity::class.java.name, hasNonRemovableEuicc)
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
    }
}
