/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher

import android.content.Context
import android.os.ServiceManager
import android.util.Log
import vendor.oplus.hardware.esim.IOplusEsim

class EsimController(private val context: Context) {
    private val oplusEsimService by lazy {
        IOplusEsim.Stub.asInterface(ServiceManager.getService("${IOplusEsim.DESCRIPTOR}/default"))
    }

    fun currentGpioState() = oplusEsimService?.esimGpio ?: 0

    fun toggleEsimState(state: Int) {
        val gpioState = currentGpioState()
        Log.d(TAG, "Current eSIM status = $gpioState")

        if (state == gpioState) {
            Log.d(TAG, "No need to change eSIM state")
            return
        }

        oplusEsimService?.setUimPower(0)
        oplusEsimService?.setEsimGpio(if (gpioState == 0) 1 else 0)
        oplusEsimService?.setUimPower(1)
    }

    companion object {
        private const val TAG = "OplusEsimController"
    }
}
