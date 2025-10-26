/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher

import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.os.ServiceManager
// import android.os.SystemProperties
import android.util.Log
import androidx.preference.*
import org.lineageos.settings.esimswitcher.R
import vendor.oplus.hardware.esim.IOplusEsim

class EsimSettingsFragment : PreferenceFragmentCompat(), Preference.OnPreferenceChangeListener {
    private val handler = Handler(Looper.getMainLooper())

    private var oplusEsim: IOplusEsim? = null

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        setPreferencesFromResource(R.xml.esim_settings, rootKey)

        oplusEsim = getOplusEsim()

        val switcher = findPreference<SwitchPreferenceCompat>(ESIM_TOGGLE_KEY)
        switcher?.onPreferenceChangeListener = this
    }

    override fun onPreferenceChange(preference: Preference, newValue: Any?): Boolean {
        handler.post { changeEsimState(if (newValue as Boolean) 1 else 0) }
        return true
    }

    @Synchronized
    private fun getOplusEsim(): IOplusEsim? {
        val fqName = IOplusEsim.DESCRIPTOR + "/default"
        return try {
            IOplusEsim.Stub.asInterface(ServiceManager.getService(fqName))
        } catch (e: Exception) {
            Log.e(TAG, "Failed to get OplusEsim service: $e")
            null
        }
    }

    private fun changeEsimState(i: Int) {
        val gpio = getEsimGpio()
        Log.d(TAG, "current eSIM status = $gpio")
        if (i == gpio) {
            Log.d(TAG, "no need to change eSIM state")
            return
        }

        // if (SystemProperties.get(ESIM_SWITCH_PROP)
        //        == "2" /* RegionLockConstants.UPDATE_AND_QUERY */) {
            setUimPower(0)
            setEsimGpio(if (gpio == 0) 1 else 0)
            setUimPower(1)
        // }
    }

    private fun getEsimGpio(): Int {
        var i = 1
        try {
            i = oplusEsim?.esimGpio ?: 1
        } catch (e: Exception) {
            Log.e(TAG, "getEsimGpio", e)
        }
        return i
    }

    private fun setEsimGpio(i: Int): Int {
        return try {
            oplusEsim?.setEsimGpio(i) ?: 1
        } catch (e: Exception) {
            Log.e(TAG, "setEsimGpio", e)
            1
        }
    }

    private fun setUimPower(i: Int): Int {
        return try {
            oplusEsim?.setUimPower(i) ?: 1
        } catch (e: Exception) {
            Log.e(TAG, "setUimPower", e)
            1
        }
    }

    companion object {
        private const val TAG = "OplusEsimSwitcher"

        // private const val ESIM_SWITCH_PROP = "ro.vendor.oplus.esim.support"
        private const val ESIM_TOGGLE_KEY = "esim_toggle"
    }
}
