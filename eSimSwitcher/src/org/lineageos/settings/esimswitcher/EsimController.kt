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
import java.util.TimerTask
import java.util.concurrent.Executor
import java.util.concurrent.TimeoutException

class EsimController(private val context: Context) {

    private val handlerExecutor = SynchronousExecutor()
    private val serviceMutex = Any()

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
    private var timerTask: TimerTask? = null
    private var connected = false
    private var omapiSeService: SEService? = null
    private var myReader: Reader? = null

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
            startSEService()
            specialSetEsimGpio(gpioState)
        } else {
            oplusEsimService?.setEsimGpio(if (gpioState == 0) 1 else 0)
        }
        oplusEsimService?.setUimPower(1)
    }

    private fun startSEService() {
        Log.d(TAG, "startSEService starting")

        omapiSeService = SEService(context, handlerExecutor,
            object : SEService.OnConnectedListener {
                override fun onConnected() {
                    synchronized(serviceMutex) {
                        Log.d(TAG, "Service onConnected")
                        connected = true
                        (serviceMutex as java.lang.Object).notify()
                    }
                }
            })

        synchronized(serviceMutex) {
            connectionTimer = Timer()
            val task = ServiceConnectionTimerTask()
            timerTask = task
            connectionTimer?.schedule(task, 3000L)
            Log.d(TAG, "startSEService done")
        }
    }

    private fun specialSetEsimGpio(state: Int) {
        Log.d(TAG, "specialSetEsimGpio starting")

        try {
            waitForConnection()
            val seService = omapiSeService
            if (seService != null && seService.isConnected) {
                val readers = seService.readers
                var session: Session? = null
                var channel: Channel? = null

                try {
                    for (reader in readers) {
                        if (reader.name == "eSE1") {
                            Log.d(TAG, "found target reader")
                            myReader = reader
                            break
                        }
                    }

                    val sessionOpen = myReader?.openSession() ?: run {
                        Log.d(TAG, "myReader == null")
                        null
                    }

                    if (sessionOpen != null) {
                        channel = sessionOpen.openLogicalChannel(null)
                    } else {
                        Log.d(TAG, "session == null")
                    }

                    //oplusEsimService?.setEsimGpio(if (state == 0) 1 else 0)
                    //oplusRadio1Service?.setModemEsimStatus(OPLUS_RIL_SERIAL, state == 0)

                    if (channel != null) {
                        channel.close()
                        sessionOpen?.close()
                    }
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
            Log.e(TAG, "waitForConnection unexpected exception: $e", e)
        } finally {
            oplusEsimService?.setEsimGpio(if (state == 0) 1 else 0)
            oplusRadio1Service?.setModemEsimStatus(OPLUS_RIL_SERIAL, state == 0)
        }
    }

    private fun waitForConnection() {
        Log.d(TAG, "waitForConnection")
        synchronized(serviceMutex) {
            if (!connected) {
                try {
                    (serviceMutex as java.lang.Object).wait()
                } catch (e: InterruptedException) {
                    Log.d(TAG, "waitForConnection: ${e.localizedMessage}")
                }
            }

            connectionTimer?.let {
                it.cancel()
                connectionTimer = null
            }

            if (!connected) {
                throw TimeoutException("Service could not be connected after 3000 ms")
            }
        }
    }

    private inner class ServiceConnectionTimerTask : TimerTask() {
        override fun run() {
            synchronized(serviceMutex) {
                (serviceMutex as java.lang.Object).notifyAll()
            }
        }
    }

    private class SynchronousExecutor : Executor {
        override fun execute(command: Runnable) = command.run()
    }

    companion object {
        private const val TAG = "OplusEsimController"

        private const val NFC_CONFIG_FILE_NAME_PROP = "persist.vendor.nfc.config_file_name"
        private const val OPLUS_ESIM_AIDL_SERVICE_NAME = "vendor.oplus.hardware.esim.IOplusEsim/default"
        private const val OPLUS_RADIO_AIDL_SERVICE_NAME_BASE = "vendor.oplus.hardware.radio.IRadioStable/OplusRadio"
        private const val OPLUS_RIL_SERIAL = 1001
    }
}
