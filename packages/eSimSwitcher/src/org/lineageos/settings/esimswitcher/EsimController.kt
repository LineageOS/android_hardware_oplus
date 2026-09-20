/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher

import android.content.Context
import android.os.ServiceManager
import android.os.SystemProperties
import android.se.omapi.SEService
import android.telephony.TelephonyManager
import android.telephony.UiccSlotMapping
import android.util.Log
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.asExecutor
import vendor.oplus.hardware.esim.IOplusEsim

class EsimController(private val context: Context) {
    private val hasSN220Chipset = SystemProperties.get(NFC_CONFIG_FILE_NAME_PROP).contains("SN220")

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

        if (hasSN220Chipset) {
            specialSetEsimGpio(if (gpioState == 0) 1 else 0)
            /* oplusEsimService?.setUimPower(1) done via SEService.OnConnectedListener */
        } else {
            oplusEsimService?.setEsimGpio(if (gpioState == 0) 1 else 0)
            oplusEsimService?.setUimPower(1)
            updateSimType(state)
        }
    }

    private fun specialSetEsimGpio(state: Int) {
        var seService: SEService? = null

        val listener =
            object : SEService.OnConnectedListener {
                override fun onConnected() {
                    Log.d(TAG, "SEService connected")

                    val service = seService ?: return

                    try {
                        val reader = service.readers.firstOrNull { it.name == "eSE1" }
                        val session = reader?.openSession()
                        val channel = session?.openLogicalChannel(null)

                        oplusEsimService?.setEsimGpio(state)
                        oplusEsimService?.setUimPower(1)
                        updateSimType(state)

                        channel?.close()
                        session?.close()
                    } catch (e: Exception) {
                        Log.e(TAG, "Failed to open eSE session", e)
                    } finally {
                        try {
                            service.shutdown()
                        } catch (e: Exception) {
                            Log.e(TAG, "Failed to shutdown SEService", e)
                        }
                    }
                }
            }

        try {
            seService = SEService(context, Dispatchers.IO.asExecutor(), listener)
        } catch (e: Exception) {
            Log.e(TAG, "Failed to start SEService", e)
        }
    }

    private fun updateSimType(state: Int) {
        try {
            val slot =
                context.resources
                    .getIntArray(com.android.internal.R.array.non_removable_euicc_slots)
                    .single()
            val telephony = context.getSystemService(TelephonyManager::class.java)
            val current = telephony.simSlotMapping
            check(current.any { it.physicalSlotIndex == slot }) { "eSIM slot is not mapped" }
            val simType =
                if (state == 1) {
                    TelephonyManager.SIM_TYPE_EMBEDDED
                } else {
                    TelephonyManager.SIM_TYPE_PHYSICAL
                }
            val updated =
                current.map {
                    if (it.physicalSlotIndex == slot) {
                        UiccSlotMapping(
                            it.portIndex,
                            it.physicalSlotIndex,
                            it.logicalSlotIndex,
                            simType,
                        )
                    } else {
                        it
                    }
                }
            if (updated.toSet() != current.toSet()) {
                telephony.setSimSlotMapping(updated)
            }
        } catch (e: RuntimeException) {
            Log.e(TAG, "Failed to update eSIM slot mapping", e)
        }
    }

    companion object {
        private const val TAG = "OplusEsimController"

        private const val NFC_CONFIG_FILE_NAME_PROP = "persist.vendor.nfc.config_file_name"
    }
}
