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

        val res = context.resources
        val euiccSlotsArrayId = res.getIdentifier("non_removable_euicc_slots", "array", "android")
        val hasNonRemovableEuicc = euiccSlotsArrayId > 0
            && (res.getIntArray(euiccSlotsArrayId)?.isNotEmpty() == true)

        if (!hasNonRemovableEuicc) {
            disableComponent(context, EsimSettingsActivity::class.java.name)
        }
    }

    private fun disableComponent(context: Context, component: String) {
        val name = ComponentName(context, component)
        val pm = context.packageManager
        pm.setComponentEnabledSetting(
            name,
            PackageManager.COMPONENT_ENABLED_STATE_DISABLED,
            PackageManager.DONT_KILL_APP
        )
    }

    companion object {
        private const val TAG = "OplusEsimBootReceiver"
    }
}
