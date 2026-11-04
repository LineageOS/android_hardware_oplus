/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.memc

import android.content.Context
import android.content.Intent
import android.os.UserHandle
import android.provider.Settings
import androidx.preference.PreferenceManager

object Utils {
    const val MEMC_ENABLED = "memc_enabled"
    const val MEMC_MODE = "memc_mode"
    const val MEMC_EXCLUDED_APPS = "memc_excluded_apps"

    private const val ENHANCED_APPS = "enhanced_apps"

    const val MEMC_MODE_60 = 0
    const val MEMC_MODE_120 = 1

    fun isMemcEnabled(context: Context) =
        Settings.Secure.getInt(context.contentResolver, MEMC_ENABLED, 0) != 0

    fun enableMemc(context: Context, enable: Boolean) {
        Settings.Secure.putInt(context.contentResolver, MEMC_ENABLED, if (enable) 1 else 0)
        checkMemcService(context)
    }

    fun getMemcMode(context: Context) =
        Settings.Secure.getInt(context.contentResolver, MEMC_MODE, MEMC_MODE_60)

    fun setMemcMode(context: Context, mode: Int) {
        Settings.Secure.putInt(context.contentResolver, MEMC_MODE, mode)
    }

    fun getExcludedApps(context: Context): Set<String> =
        Settings.Secure.getString(context.contentResolver, MEMC_EXCLUDED_APPS)
            ?.split(",")
            ?.filter { it.isNotEmpty() }
            ?.toSet()
            .orEmpty()

    fun setAppExcluded(context: Context, packageName: String, excluded: Boolean) {
        val apps = getExcludedApps(context).toMutableSet()
        if (excluded) apps.add(packageName) else apps.remove(packageName)
        Settings.Secure.putString(
            context.contentResolver,
            MEMC_EXCLUDED_APPS,
            apps.joinToString(","),
        )
    }

    fun getEnhancedApps(context: Context): Set<String> =
        PreferenceManager.getDefaultSharedPreferences(context)
            .getStringSet(ENHANCED_APPS, null)
            .orEmpty()

    fun addEnhancedApp(context: Context, packageName: String) {
        val apps = getEnhancedApps(context)
        if (packageName !in apps) {
            PreferenceManager.getDefaultSharedPreferences(context)
                .edit()
                .putStringSet(ENHANCED_APPS, apps + packageName)
                .apply()
        }
    }

    fun checkMemcService(context: Context) {
        val intent = Intent(context, MemcService::class.java)
        if (isMemcEnabled(context)) {
            context.startServiceAsUser(intent, UserHandle.CURRENT)
        } else {
            context.stopServiceAsUser(intent, UserHandle.CURRENT)
        }
    }
}
