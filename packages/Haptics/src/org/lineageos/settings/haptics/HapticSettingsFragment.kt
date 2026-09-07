/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.haptics

import android.os.Bundle
import androidx.preference.ListPreference
import androidx.preference.Preference
import com.android.settingslib.widget.SettingsBasePreferenceFragment

class HapticSettingsFragment :
    SettingsBasePreferenceFragment(), Preference.OnPreferenceChangeListener {

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        setPreferencesFromResource(R.xml.haptic_settings, rootKey)

        val pref = findPreference<ListPreference>("haptic_style")
        pref?.let {
            it.value = HapticUtils.getTouchStyle().toString()
            it.onPreferenceChangeListener = this
        }
    }

    override fun onPreferenceChange(preference: Preference, newValue: Any): Boolean {
        if (preference.key == "haptic_style") {
            val style = (newValue as String).toIntOrNull() ?: HapticUtils.STYLE_CRISP
            HapticUtils.setTouchStyle(requireContext(), style)
            return true
        }
        return false
    }
}
