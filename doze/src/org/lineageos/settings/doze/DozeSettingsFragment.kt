/*
 * Copyright (C) 2021 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.doze

import android.app.Activity
import android.app.AlertDialog
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.LayoutInflater
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import android.widget.CompoundButton
import android.widget.Switch
import android.widget.TextView
import androidx.preference.*

class DozeSettingsFragment : PreferenceFragment(), Preference.OnPreferenceChangeListener,
    CompoundButton.OnCheckedChangeListener {
    private lateinit var alwaysOnDisplayPreference: SwitchPreference
    private lateinit var switchBar: View
    private lateinit var textView: TextView

    private var pickUpPreference: ListPreference? = null
    private var pocketPreference: SwitchPreference? = null

    private val handler = Handler(Looper.getMainLooper())

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        addPreferencesFromResource(R.xml.doze_settings)
        activity.actionBar?.setDisplayHomeAsUpEnabled(true)

        val prefs = activity.getSharedPreferences("doze_settings", Activity.MODE_PRIVATE)!!
        if (savedInstanceState == null && !prefs.getBoolean("first_help_shown", false)) {
            AlertDialog.Builder(context)
                .setTitle(R.string.doze_settings_help_title)
                .setMessage(R.string.doze_settings_help_text)
                .setNegativeButton(R.string.dialog_ok) { _, _ ->
                    prefs.edit().putBoolean("first_help_shown", true).apply()
                }
                .show()
        }

        val dozeEnabled = Utils.isDozeEnabled(context)
        alwaysOnDisplayPreference = findPreference(Utils.ALWAYS_ON_DISPLAY)!!
        alwaysOnDisplayPreference.isEnabled = dozeEnabled
        alwaysOnDisplayPreference.isChecked = Utils.isAlwaysOnEnabled(context)
        alwaysOnDisplayPreference.onPreferenceChangeListener = this

        val pickupSensorCategory =
            preferenceScreen.findPreference<PreferenceCategory>(Utils.CATEGORY_PICKUP_SENSOR)
        if (getString(R.string.pickup_sensor_type).isEmpty()) {
            preferenceScreen.removePreference(pickupSensorCategory)
        }

        val proximitySensorCategory =
            preferenceScreen.findPreference<PreferenceCategory>(Utils.CATEGORY_PROXIMITY_SENSOR)
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
        if (!Utils.alwaysOnDisplayAvailable(context)) {
            preferenceScreen.removePreference(alwaysOnDisplayPreference)
        } else {
            pickupSensorCategory?.dependency = Utils.ALWAYS_ON_DISPLAY
            proximitySensorCategory?.dependency = Utils.ALWAYS_ON_DISPLAY
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?
    ): View {
        val view = LayoutInflater.from(context).inflate(R.layout.doze, container, false)
        (view as ViewGroup).addView(super.onCreateView(inflater, container, savedInstanceState))
        return view
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        val dozeEnabled = Utils.isDozeEnabled(context)

        textView = view.findViewById(R.id.switch_text)
        textView.text =
            getString(if (dozeEnabled) R.string.switch_bar_on else R.string.switch_bar_off)
        switchBar = view.findViewById(R.id.switch_bar)

        val switchWidget = switchBar.findViewById<Switch>(android.R.id.switch_widget)
        switchWidget.isChecked = dozeEnabled
        switchWidget.setOnCheckedChangeListener(this)

        switchBar.isActivated = dozeEnabled
        switchBar.setOnClickListener {
            switchWidget.isChecked = !switchWidget.isChecked
            switchBar.isActivated = switchWidget.isChecked
        }
    }

    override fun onPreferenceChange(preference: Preference, newValue: Any?): Boolean {
        if (preference.key == Utils.ALWAYS_ON_DISPLAY) {
            Utils.enableAlwaysOn(context, newValue as Boolean)
        }
        handler.post { Utils.checkDozeService(context) }
        return true
    }

    override fun onCheckedChanged(compoundButton: CompoundButton, isChecked: Boolean) {
        Utils.enableDoze(context, isChecked)
        Utils.checkDozeService(context)

        textView.text =
            getString(if (isChecked) R.string.switch_bar_on else R.string.switch_bar_off)
        switchBar.isActivated = isChecked

        if (!isChecked) {
            Utils.enableAlwaysOn(context, false)
            alwaysOnDisplayPreference.isChecked = false
        }

        alwaysOnDisplayPreference.isEnabled = isChecked
        pickUpPreference?.isEnabled = isChecked
        pocketPreference?.isEnabled = isChecked
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        return when (item.itemId) {
            R.id.home -> {
                activity.onBackPressed()
                true
            }
            else -> false
        }
    }
}
