/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher

import android.app.AlertDialog
import android.os.Bundle
import androidx.preference.Preference
import androidx.preference.SwitchPreferenceCompat
import com.android.settingslib.widget.SettingsBasePreferenceFragment
import org.lineageos.settings.esimswitcher.R

class EsimSettingsFragment : SettingsBasePreferenceFragment(), Preference.OnPreferenceChangeListener {
    private lateinit var controller: EsimController

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        setPreferencesFromResource(R.xml.esim_settings, rootKey)
        controller = EsimController(requireContext())

        val switcher = findPreference<SwitchPreferenceCompat>(ESIM_TOGGLE_KEY)
        switcher?.isChecked = controller.currentGpioState() == 1
        switcher?.onPreferenceChangeListener = this
    }

    override fun onPreferenceChange(preference: Preference, newValue: Any?): Boolean {
        val enable = newValue as Boolean
        if (enable) {
            AlertDialog.Builder(requireContext())
                .setMessage(R.string.esim_toggle_dialog)
                .setNegativeButton(R.string.esim_toggle_dialog_cancel) { dialog, _ ->
                    dialog.dismiss()
                }
                .setPositiveButton(R.string.esim_toggle_dialog_ok) { dialog, _ ->
                    dialog.dismiss()
                    controller.toggleEsimState(1)
                    (preference as? SwitchPreferenceCompat)?.isChecked = true
                }
                .show()
            return false
        } else {
            controller.toggleEsimState(0)
            return true
        }
    }

    companion object {
        private const val ESIM_TOGGLE_KEY = "esim_toggle"
    }
}
