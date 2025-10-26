/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher

import android.app.AlertDialog
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
import vendor.oplus.hardware.radio.IOplusRadio

class EsimSettingsFragment : PreferenceFragmentCompat(), Preference.OnPreferenceChangeListener {
    private val handler = Handler(Looper.getMainLooper())

    private val oplusEsimService by lazy {
        IOplusEsim.Stub.asInterface(ServiceManager.getService(OPLUS_ESIM_AIDL_SERVICE_NAME))
    }

    private val oplusRadio1Service by lazy {
        IOplusRadio.Stub.asInterface(ServiceManager.getService(OPLUS_RADIO_AIDL_SERVICE_NAME_BASE + 1))
    }

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        setPreferencesFromResource(R.xml.esim_settings, rootKey)

        val switcher = findPreference<SwitchPreferenceCompat>(ESIM_TOGGLE_KEY)
        switcher?.isChecked = oplusEsimService?.esimGpio == 1
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
                    handler.post { changeEsimState(1) }
                    (preference as? SwitchPreferenceCompat)?.isChecked = true
                }
                .show()

            return false
        } else {
            handler.post { changeEsimState(0) }
            return true
        }
    }

    private fun changeEsimState(state: Int) {
        val gpio = oplusEsimService?.esimGpio ?: return
        Log.d(TAG, "Current eSIM status = $gpio")

        if (state == gpio) {
            Log.d(TAG, "No need to change eSIM state")
            return
        }

        oplusEsimService.setUimPower(0)
        oplusEsimService.setEsimGpio(if (gpio == 0) 1 else 0)
        oplusRadio1Service?.setModemEsimStatus(OPLUS_RIL_SERIAL, gpio == 0)
        oplusEsimService.setUimPower(1)
    }

    companion object {
        private const val TAG = "OplusEsimSwitcher"

        private const val ESIM_TOGGLE_KEY = "esim_toggle"
        private const val OPLUS_ESIM_AIDL_SERVICE_NAME = "vendor.oplus.hardware.esim.IOplusEsim/default"
        private const val OPLUS_RADIO_AIDL_SERVICE_NAME_BASE = "vendor.oplus.hardware.radio.IRadioStable/OplusRadio"
        private const val OPLUS_RIL_SERIAL = 1001
    }
}
