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
import java.io.IOException
import java.util.Timer
import java.util.concurrent.Executor
import java.util.concurrent.TimeoutException
import kotlin.concurrent.schedule

class EsimController(private val context: Context) {

    private val serviceMutex = Object()

    private val nfcConf by lazy {
        SystemProperties.get(NFC_CONFIG_FILE_NAME_PROP)
    }

    private val oplusEsimService by lazy {
        IOplusEsim.Stub.asInterface(ServiceManager.getService(OPLUS_ESIM_AIDL_SERVICE_NAME))
    }

    private val oplusRadio1Service by lazy {
        IOplusRadio.Stub.asInterface(ServiceManager.getService(OPLUS_RADIO_AIDL_SERVICE_NAME_BASE + 1))
    }

    private var connectionTimer: Timer? = null
    private var connected = false

    fun currentGpioState(): Int {
        return oplusEsimService?.esimGpio ?: 0
    }

    fun toggleEsimState(state: Int) {
        val gpioState = currentGpioState()
        Log.d(TAG, "Current eSIM status = $gpioState")

        if (state == gpioState) {
            Log.d(TAG, "No need to change eSIM state")
            return
        }

        oplusEsimService?.setUimPower(0)
        if (nfcConf.contains("SN220")) {
            specialSetEsimGpio(gpioState)
        } else {
            oplusEsimService?.setEsimGpio(if (gpioState == 0) 1 else 0)
        }
        oplusEsimService?.setUimPower(1)
    }

    private fun specialSetEsimGpio(state: Int) {
        Log.d(TAG, "specialSetEsimGpio starting")

        try {
            val seService = SEService(
                context,
                object : Executor {
                    override fun execute(command: Runnable) = command.run()
                },
                object : SEService.OnConnectedListener {
                    override fun onConnected() = synchronized(serviceMutex) {
                        Log.d(TAG, "Service onConnected")
                        connected = true
                        serviceMutex.notify()
                    }
                }
            )

            synchronized(serviceMutex) {
                connectionTimer = Timer()
                connectionTimer?.schedule(3000L) {
                    synchronized(serviceMutex) {
                        serviceMutex.notifyAll()
                    }
                }

                if (!connected) {
                    serviceMutex.wait()
                }

                connectionTimer?.cancel()
                connectionTimer = null

                if (!connected) {
                    throw TimeoutException("Service could not be connected after 3000 ms")
                }
            }

            if (seService?.isConnected == true) {
                try {
                    val reader = seService.readers.firstOrNull { it.name == "eSE1" }
                    val sessionOpen = reader?.openSession()
                    val channel = sessionOpen?.openLogicalChannel(null)

                    channel?.close()
                    sessionOpen?.close()
                } catch (e: IOException) {
                    Log.e(TAG, "IOException: ${e.message}", e)
                } catch (e: IllegalStateException) {
                    Log.e(TAG, "IllegalStateException: ${e.message}", e)
                } finally {
                    seService.shutdown()
                    connected = false
                    Log.d(TAG, "SE Service was shut down")
                }
            }
        } catch (e: Exception) {
            Log.e(TAG, "specialSetEsimGpio unexpected exception: $e", e)
        } finally {
            oplusEsimService?.setEsimGpio(if (state == 0) 1 else 0)
            oplusRadio1Service?.setModemEsimStatus(OPLUS_RIL_SERIAL, state == 0)
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
