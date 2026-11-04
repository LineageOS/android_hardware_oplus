/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.memc

import android.content.pm.ApplicationInfo
import android.content.pm.PackageManager
import android.os.Bundle
import androidx.preference.SwitchPreferenceCompat
import com.android.settingslib.widget.FooterPreference
import com.android.settingslib.widget.SettingsBasePreferenceFragment

class MemcAppsFragment : SettingsBasePreferenceFragment() {
    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        val context = requireContext()
        val packageManager = context.packageManager
        val excludedApps = Utils.getExcludedApps(context)
        preferenceScreen = preferenceManager.createPreferenceScreen(context)

        Utils.getEnhancedApps(context)
            .mapNotNull { getApplicationInfo(packageManager, it) }
            .map { it to it.loadLabel(packageManager).toString() }
            .sortedBy { (_, label) -> label.lowercase() }
            .forEach { (info, label) ->
                preferenceScreen.addPreference(
                    SwitchPreferenceCompat(context).apply {
                        key = info.packageName
                        title = label
                        icon = info.loadIcon(packageManager)
                        isPersistent = false
                        isChecked = info.packageName !in excludedApps
                        setOnPreferenceChangeListener { _, newValue ->
                            Utils.setAppExcluded(context, info.packageName, !(newValue as Boolean))
                            true
                        }
                    }
                )
            }

        if (preferenceScreen.preferenceCount == 0) {
            preferenceScreen.addPreference(
                FooterPreference(context).apply { title = getString(R.string.memc_apps_empty) }
            )
        }
    }

    private fun getApplicationInfo(
        packageManager: PackageManager,
        packageName: String,
    ): ApplicationInfo? =
        try {
            packageManager.getApplicationInfo(packageName, 0)
        } catch (e: PackageManager.NameNotFoundException) {
            null
        }
}
