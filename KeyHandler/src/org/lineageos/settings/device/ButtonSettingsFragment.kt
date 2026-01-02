/*
 * SPDX-FileCopyrightText: 2021-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device

import android.app.NotificationManager
import android.content.Context
import android.os.Bundle
import android.util.Log
import androidx.preference.ListPreference
import androidx.preference.Preference
import androidx.preference.PreferenceFragmentCompat

class ButtonSettingsFragment : PreferenceFragmentCompat() {

    private lateinit var mContext: Context

    override fun onAttach(context: Context) {
        super.onAttach(context)
        mContext = context
    }

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        setPreferencesFromResource(R.xml.button_panel, rootKey)

        // 1. Get the list of DND modes and their values from the system
        val (dndEntries, dndEntryValues) = getDndModes()
        val entriesArray = dndEntries.toTypedArray()
        val valuesArray = dndEntryValues.toTypedArray()

        // 2. Set up the logic for all three switch positions
        //    This wires up the controller (action) to the dependent (dnd mode) and ringer mode
        setupPreferenceLogic(
            KEY_POSITION_TOP_ACTION,
            KEY_POSITION_TOP_DND,
            KEY_POSITION_TOP_RINGER,
            entriesArray,
            valuesArray,
        )
        setupPreferenceLogic(
            KEY_POSITION_MIDDLE_ACTION,
            KEY_POSITION_MIDDLE_DND,
            KEY_POSITION_MIDDLE_RINGER,
            entriesArray,
            valuesArray,
        )
        setupPreferenceLogic(
            KEY_POSITION_BOTTOM_ACTION,
            KEY_POSITION_BOTTOM_DND,
            KEY_POSITION_BOTTOM_RINGER,
            entriesArray,
            valuesArray,
        )
    }

    /** Wires up a controller preference to a dependent DND preference. */
    private fun setupPreferenceLogic(
        controllerKey: String,
        dependentKey: String,
        ringerKey: String,
        dndEntries: Array<CharSequence>,
        dndEntryValues: Array<CharSequence>,
    ) {
        val controllerPref: ListPreference? = findPreference(controllerKey)
        val dependentDndPref: ListPreference? = findPreference(dependentKey)
        val ringerPref: ListPreference? = findPreference(ringerKey)

        if (controllerPref == null || dependentDndPref == null || ringerPref == null) {
            return
        }

        // 3. Populate the dependent preference with our dynamic DND modes
        dependentDndPref.entries = dndEntries
        dependentDndPref.entryValues = dndEntryValues

        // 4. Set the listener on the controller
        controllerPref.setOnPreferenceChangeListener { _, newValue ->
            // When the controller changes, update the dependent's visibility
            updateDndPreferenceVisibility(dependentDndPref, newValue.toString())
            updateRingerPreferenceVisibility(ringerPref, newValue.toString())
            true // Accept the change
        }

        // 5. Set the initial visibility when the screen loads
        updateDndPreferenceVisibility(dependentDndPref, controllerPref.value)
        updateRingerPreferenceVisibility(ringerPref, controllerPref.value)
    }

    private fun updateRingerPreferenceVisibility(
        ringerPreference: Preference,
        controllerValue: String?,
    ) {
        // Show the Ringer preference ONLY if the selected action is of ZEN_PRIORITY_ONLY,
        // ZEN_ALARMS_ONLY, or DND_MODE
        ringerPreference.isVisible =
            (controllerValue == ZEN_PRIORITY_ONLY_ACTION ||
                controllerValue == ZEN_ALARMS_ONLY_ACTION ||
                controllerValue == VALUE_DND_MODE_ACTION)
        // set default to normal mode (0) if it becomes visible and has no value
        if (ringerPreference.isVisible) {
            val listPref = ringerPreference as ListPreference
            if (listPref.value == null || listPref.value!!.isEmpty()) {
                val defaultRingerValue = "0" // Normal mode
                listPref.value = defaultRingerValue
                Log.d(
                    TAG,
                    "Ringer Preference ${ringerPreference.key} value set to $defaultRingerValue",
                )
            }
        }
    }

    /** Shows or hides the DND preference based on the controller's value. */
    private fun updateDndPreferenceVisibility(dndPreference: Preference, controllerValue: String?) {
        // Show the DND preference ONLY if the selected action is "dnd_mode"
        dndPreference.isVisible = (controllerValue == VALUE_DND_MODE_ACTION)
        // set the sharedPreferences to the first element in the dndEntryValues if it becomes
        // visible and make that the default for the preference
        if (dndPreference.isVisible) {
            val listPref = dndPreference as ListPreference
            if (listPref.value == null || listPref.value!!.isEmpty()) {
                val firstValue = listPref.entryValues[0].toString()
                listPref.value = firstValue
            }
        }
    }

    /** Fetches all user-created automatic (Zen) rules */
    private fun getDndModes(): Pair<List<CharSequence>, List<CharSequence>> {
        val notificationManager =
            mContext.getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager

        val entries = mutableListOf<CharSequence>()
        val entryValues = mutableListOf<CharSequence>()

        notificationManager.getAutomaticZenRules().entries.forEach { (id, rule) ->
            entries.add(rule.name)
            entryValues.add(id)
        }

        Log.d(TAG, "Fetched ${entries.size} DND modes")
        Log.d(TAG, "Entries: ${entries.joinToString()}")
        return Pair(entries, entryValues)
    }

    companion object {
        const val VALUE_DND_MODE_ACTION =
            "10" // value from XML (arrays.xml) 10 was chosen since it is far enough from other
        // values to not be confusing
        private const val ZEN_PRIORITY_ONLY_ACTION = "3"
        private const val ZEN_ALARMS_ONLY_ACTION = "5"

        // Keys for Top Position
        const val KEY_POSITION_TOP_ACTION = "config_top_position"
        const val KEY_POSITION_TOP_DND = "config_top_zen_mode"
        const val KEY_POSITION_TOP_RINGER = "config_top_ringer_mode"

        // Keys for Middle Position
        const val KEY_POSITION_MIDDLE_ACTION = "config_middle_position"
        const val KEY_POSITION_MIDDLE_DND = "config_middle_zen_mode"
        const val KEY_POSITION_MIDDLE_RINGER = "config_middle_ringer_mode"

        // Keys for Bottom Position
        const val KEY_POSITION_BOTTOM_ACTION = "config_bottom_position"
        const val KEY_POSITION_BOTTOM_DND = "config_bottom_zen_mode"
        const val KEY_POSITION_BOTTOM_RINGER = "config_bottom_ringer_mode"

        const val TAG = "ButtonSettingsFragment"
    }
}
