/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher

import android.content.Context
import android.os.ServiceManager
import android.os.SystemProperties
import android.se.omapi.Channel
import android.se.omapi.Reader
import android.se.omapi.SEService
import android.se.omapi.Session
import android.util.Log
import vendor.oplus.hardware.esim.IOplusEsim
import vendor.oplus.hardware.radio.IOplusRadio
import java.util.concurrent.Executor

class EsimController(private val context: Context) {
    private val oplusEsimService by lazy {
        IOplusEsim.Stub.asInterface(ServiceManager.getService(OPLUS_ESIM_AIDL_SERVICE_NAME))
    }

    private val oplusRadio1Service by lazy {
        IOplusRadio.Stub.asInterface(ServiceManager.getService(OPLUS_RADIO_AIDL_SERVICE_NAME_BASE + 1))
    }

    val hasSN220Chipset = SystemProperties.get(NFC_CONFIG_FILE_NAME_PROP).contains("SN220")

    fun currentGpioState() = oplusEsimService?.esimGpio ?: 0

    fun setUimPower(state: Int) = oplusEsimService?.setUimPower(state)

    fun toggleEsimState(state: Int) {
        val gpioState = currentGpioState()
        Log.d(TAG, "Current eSIM status = $gpioState")

        if (state == gpioState) {
            Log.d(TAG, "No need to change eSIM state")
            return
        }

        setUimPower(0)
        setEsimGpio(if (gpioState == 0) 1 else 0)
    }

    private fun setEsimGpio(state: Int) {
        if (hasSN220Chipset) {
            specialSetEsimGpio(state)
        } else {
            oplusEsimService?.setEsimGpio(state)
            setUimPower(1)
        }
    }

    private fun specialSetEsimGpio(state: Int) {
        try {
            var seService: SEService? = null

            val listener = object : SEService.OnConnectedListener {
                override fun onConnected() {
                    Log.d(TAG, "SEService connected")

                    val service = seService ?: return

                    try {
                        val reader = service.readers.firstOrNull { it.name == "eSE1" }
                        val session = reader?.openSession()
                        val channel = session?.openLogicalChannel(null)

                        Log.d(TAG, "Successfully opened eSE session, applying modem hooks")
                        oplusEsimService?.setEsimGpio(state)
                        oplusRadio1Service?.setModemEsimStatus(OPLUS_RIL_SERIAL, state == 1)
                        setUimPower(1)

                        channel?.close()
                        session?.close()
                    } catch (e: Exception) {
                        Log.e(TAG, "specialSetEsimGpio failed", e)
                    } finally {
                        try {
                            service.shutdown()
                        } catch (e: Exception) {
                            Log.e(TAG, "Failed to shutdown SEService")
                        }
                    }
                }
            }

            val executor = Executor { command -> Thread(command).start() }

            seService = SEService(context, executor, listener)
        } catch (e: Exception) {
            Log.e(TAG, "specialSetEsimGpio failed", e)
        }
    }

    companion object {
        private const val TAG = "OplusEsimController"

        private const val NFC_CONFIG_FILE_NAME_PROP = "persist.vendor.nfc.config_file_name"
        private const val OPLUS_ESIM_AIDL_SERVICE_NAME = "vendor.oplus.hardware.esim.IOplusEsim/default"
        private const val OPLUS_RADIO_AIDL_SERVICE_NAME_BASE = "vendor.oplus.hardware.radio.IRadioStable/OplusRadio"
        private const val OPLUS_RIL_SERIAL = 1001
    }
}
