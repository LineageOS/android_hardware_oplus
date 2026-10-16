/*
 * SPDX-FileCopyrightText: 2025-2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher

import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.lifecycleScope
import androidx.lifecycle.repeatOnLifecycle
import androidx.preference.Preference
import androidx.preference.SwitchPreferenceCompat
import com.android.settingslib.widget.SettingsBasePreferenceFragment
import kotlinx.coroutines.launch

class EsimSettingsFragment :
    SettingsBasePreferenceFragment(), Preference.OnPreferenceChangeListener {
    private lateinit var controller: EsimController

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        setPreferencesFromResource(R.xml.esim_settings, rootKey)
        controller = EsimController(requireContext())

        val switcher = findPreference<SwitchPreferenceCompat>(ESIM_TOGGLE_KEY)
        switcher?.isChecked = controller.currentGpioState() == 1
        switcher?.onPreferenceChangeListener = this
        lifecycleScope.launch {
            repeatOnLifecycle(Lifecycle.State.STARTED) {
                controller.pendingState.collect { state ->
                    switcher?.isEnabled = state == null
                    switcher?.setSummary(
                        when (state) {
                            1 -> R.string.esim_toggle_enabling
                            0 -> R.string.esim_toggle_disabling
                            else -> R.string.esim_toggle_summary
                        }
                    )
                    if (state == null) switcher?.isChecked = controller.currentGpioState() == 1
                }
            }
        }
    }

    override fun onPreferenceChange(preference: Preference, newValue: Any?): Boolean {
        val enable = newValue as Boolean
        if (enable) {
            val impact = controller.getEnableImpact()
            val message =
                when {
                    impact == null -> getString(R.string.esim_enable_warning_unknown)
                    impact.disablesAllPhysicalSims ->
                        getString(R.string.esim_enable_warning_multiple)
                    else ->
                        getString(R.string.esim_enable_warning_single, impact.physicalSlotIndex + 1)
                }
            AlertDialog.Builder(requireContext())
                .setMessage(message)
                .setNegativeButton(R.string.esim_toggle_dialog_cancel) { dialog, _ ->
                    dialog.dismiss()
                }
                .setPositiveButton(R.string.esim_toggle_dialog_continue) { dialog, _ ->
                    dialog.dismiss()
                    toggleEsimState(true)
                }
                .show()
            return false
        } else {
            toggleEsimState(false)
            return false
        }
    }

    private fun toggleEsimState(enable: Boolean) {
        val context = requireContext().applicationContext
        controller.toggleEsimState(if (enable) 1 else 0) { success ->
            if (!success) {
                Toast.makeText(context, R.string.esim_switch_failed, Toast.LENGTH_LONG).show()
            }
        }
    }

    companion object {
        private const val ESIM_TOGGLE_KEY = "esim_toggle"
    }
}
