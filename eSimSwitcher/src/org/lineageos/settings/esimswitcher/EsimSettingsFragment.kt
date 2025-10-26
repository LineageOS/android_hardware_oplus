/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher

import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.os.ServiceManager
import android.util.Log
import androidx.preference.Preference
import androidx.preference.PreferenceFragmentCompat
import androidx.preference.SwitchPreferenceCompat
import org.lineageos.settings.esimswitcher.R
import vendor.oplus.hardware.esim.IOplusEsim

class EsimSettingsFragment : PreferenceFragmentCompat(), Preference.OnPreferenceChangeListener {
    private val handler = Handler(Looper.getMainLooper())

    private val oplusEsim by lazy {
        IOplusEsim.Stub.asInterface(ServiceManager.getService("${IOplusEsim.DESCRIPTOR}/default"))
    }

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        setPreferencesFromResource(R.xml.esim_settings, rootKey)

        val switcher = findPreference<SwitchPreferenceCompat>(ESIM_TOGGLE_KEY)
        switcher?.onPreferenceChangeListener = this
    }

    override fun onPreferenceChange(preference: Preference, newValue: Any?): Boolean {
        handler.post {
            changeEsimState(if (newValue as Boolean) 1 else 0)
        }

        return true
    }

    private fun changeEsimState(state: Int) {
        val gpio = oplusEsim?.esimGpio ?: return
        Log.d(TAG, "Current eSIM status = $gpio")

        if (state == gpio) {
            Log.d(TAG, "No need to change eSIM state")
            return
        }

        oplusEsim.setUimPower(0)
        oplusEsim.setEsimGpio(if (gpio == 0) 1 else 0)
        oplusEsim.setUimPower(1)
    }

    companion object {
        private const val TAG = "OplusEsimSwitcher"

        private const val ESIM_TOGGLE_KEY = "esim_toggle"
    }
}
