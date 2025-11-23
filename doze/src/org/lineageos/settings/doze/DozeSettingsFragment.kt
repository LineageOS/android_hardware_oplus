/*
 * SPDX-FileCopyrightText: 2021-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.doze

import android.app.Activity
import android.app.AlertDialog
import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import androidx.preference.ListPreference
import androidx.preference.Preference
import androidx.preference.PreferenceCategory
import androidx.preference.SwitchPreferenceCompat
import com.android.settingslib.widget.MainSwitchPreference
import com.android.settingslib.widget.SettingsBasePreferenceFragment

class DozeSettingsFragment : SettingsBasePreferenceFragment(), Preference.OnPreferenceChangeListener {
    private lateinit var alwaysOnDisplayPreference: SwitchPreferenceCompat

    private var pickUpPreference: ListPreference? = null
    private var pocketPreference: SwitchPreferenceCompat? = null

    private val handler = Handler(Looper.getMainLooper())

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        setPreferencesFromResource(R.xml.doze_settings, rootKey)

        val prefs = requireActivity().getSharedPreferences("doze_settings", Context.MODE_PRIVATE)
        if (savedInstanceState == null && !prefs.getBoolean("first_help_shown", false)) {
            AlertDialog.Builder(requireContext())
                .setTitle(R.string.doze_settings_help_title)
                .setMessage(R.string.doze_settings_help_text)
                .setNegativeButton(R.string.dialog_ok) { _, _ ->
                    prefs.edit().putBoolean("first_help_shown", true).apply()
                }
                .show()
        }

        val dozeEnabled = Utils.isDozeEnabled(requireContext())
        val switchBar = findPreference<MainSwitchPreference>(Utils.DOZE_ENABLE)!!
        switchBar.onPreferenceChangeListener = this
        switchBar.isChecked = dozeEnabled

        alwaysOnDisplayPreference = findPreference(Utils.ALWAYS_ON_DISPLAY)!!
        alwaysOnDisplayPreference.isEnabled = dozeEnabled
        alwaysOnDisplayPreference.isChecked = Utils.isAlwaysOnEnabled(requireContext())
        alwaysOnDisplayPreference.onPreferenceChangeListener = this

        val pickupSensorCategory =
            preferenceScreen.findPreference<PreferenceCategory>(Utils.CATEGORY_PICKUP_SENSOR)!!
        if (getString(R.string.pickup_sensor_type).isEmpty()) {
            preferenceScreen.removePreference(pickupSensorCategory)
        }

        val proximitySensorCategory =
            preferenceScreen.findPreference<PreferenceCategory>(Utils.CATEGORY_PROXIMITY_SENSOR)!!
        if (getString(R.string.pocket_sensor_type).isEmpty()) {
            preferenceScreen.removePreference(proximitySensorCategory)
        }

        pickUpPreference = findPreference(Utils.GESTURE_PICK_UP_KEY)
        pickUpPreference?.isEnabled = dozeEnabled
        pickUpPreference?.onPreferenceChangeListener = this

        pocketPreference = findPreference(Utils.GESTURE_POCKET_KEY)
        pocketPreference?.isEnabled = dozeEnabled
        pocketPreference?.onPreferenceChangeListener = this

        // Hide AOD if not supported and set all its dependents otherwise
        if (!Utils.alwaysOnDisplayAvailable(requireContext())) {
            preferenceScreen.removePreference(alwaysOnDisplayPreference)
        } else {
            pickupSensorCategory.dependency = Utils.ALWAYS_ON_DISPLAY
            proximitySensorCategory.dependency = Utils.ALWAYS_ON_DISPLAY
        }
    }

    override fun onPreferenceChange(preference: Preference, newValue: Any?): Boolean {
        val isChecked = newValue as Boolean
        when (preference.key) {
            Utils.ALWAYS_ON_DISPLAY -> {
                Utils.enableAlwaysOn(requireContext(), isChecked)
            }
            Utils.DOZE_ENABLE -> {
                Utils.enableDoze(requireContext(), isChecked)
                Utils.checkDozeService(requireContext())

                if (!isChecked) {
                    Utils.enableAlwaysOn(requireContext(), false)
                    alwaysOnDisplayPreference.isChecked = false
                }

                alwaysOnDisplayPreference.isEnabled = isChecked
                pickUpPreference?.isEnabled = isChecked
                pocketPreference?.isEnabled = isChecked
            }
        }

        handler.post { Utils.checkDozeService(requireContext()) }
        return true
    }

}
