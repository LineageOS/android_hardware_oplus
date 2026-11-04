/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.memc

import android.os.Bundle
import androidx.preference.ListPreference
import androidx.preference.Preference
import com.android.settingslib.widget.MainSwitchPreference
import com.android.settingslib.widget.SettingsBasePreferenceFragment

class MemcSettingsFragment :
    SettingsBasePreferenceFragment(), Preference.OnPreferenceChangeListener {
    private lateinit var appsPreference: Preference
    private lateinit var modePreference: ListPreference

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        setPreferencesFromResource(R.xml.memc_settings, rootKey)

        val enabled = Utils.isMemcEnabled(requireContext())
        val switchBar = findPreference<MainSwitchPreference>(Utils.MEMC_ENABLED)!!
        switchBar.onPreferenceChangeListener = this
        switchBar.isChecked = enabled

        modePreference = findPreference(Utils.MEMC_MODE)!!
        modePreference.onPreferenceChangeListener = this
        modePreference.isEnabled = enabled
        updateMode(Utils.getMemcMode(requireContext()))

        appsPreference = findPreference(KEY_APPS)!!
        appsPreference.isEnabled = enabled
    }

    override fun onPreferenceChange(preference: Preference, newValue: Any?): Boolean {
        when (preference.key) {
            Utils.MEMC_ENABLED -> {
                val enabled = newValue as Boolean
                Utils.enableMemc(requireContext(), enabled)
                modePreference.isEnabled = enabled
                appsPreference.isEnabled = enabled
            }
            Utils.MEMC_MODE -> {
                val mode = (newValue as String).toInt()
                Utils.setMemcMode(requireContext(), mode)
                updateMode(mode)
            }
        }
        return true
    }

    private fun updateMode(mode: Int) {
        modePreference.value = mode.toString()
        modePreference.summary =
            if (mode == Utils.MEMC_MODE_120) {
                getString(R.string.memc_effect_120) +
                    "\n" +
                    getString(R.string.memc_effect_120_summary)
            } else {
                getString(R.string.memc_effect_60)
            }
    }

    companion object {
        private const val KEY_APPS = "memc_apps"
    }
}
