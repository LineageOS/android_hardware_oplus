/*
 * SPDX-FileCopyrightText: 2025-2026 The LineageOS Project
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
import java.util.concurrent.atomic.AtomicInteger
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.asExecutor
import vendor.oplus.hardware.esim.IOplusEsim
import vendor.oplus.hardware.subsys_interface.subsys_radio.ISubsysRadio

class EsimController(private val context: Context) {
    private val hasSN220Chipset = SystemProperties.get(NFC_CONFIG_FILE_NAME_PROP).contains("SN220")

    private val oplusEsimService by lazy {
        IOplusEsim.Stub.asInterface(ServiceManager.getService("${IOplusEsim.DESCRIPTOR}/default"))
    }

    fun currentGpioState() = oplusEsimService?.esimGpio ?: 0

    fun toggleEsimState(state: Int, onComplete: (Boolean) -> Unit) {
        switchEsimState(state, false, onComplete)
    }

    private fun switchEsimState(state: Int, force: Boolean, onComplete: (Boolean) -> Unit) {
        val complete: (Boolean) -> Unit = { success ->
            context.mainExecutor.execute { onComplete(success) }
        }
        try {
            checkNotNull(oplusEsimService) { "eSIM service is unavailable" }
            val gpioState = currentGpioState()
            Log.d(TAG, "Current eSIM status = $gpioState")

            if (!force && state == gpioState) {
                Log.d(TAG, "No need to change eSIM state")
                complete(true)
                return
            }

            val slotMapping = if (hasSN220Chipset) getSwitchableSlotMapping() else null
            oplusEsimService?.setUimPower(0)

            if (hasSN220Chipset) {
                specialSetEsimGpio(state, slotMapping, complete)
                /* oplusEsimService?.setUimPower(1) done via SEService.OnConnectedListener */
            } else {
                updateModemEsimState(state)
                oplusEsimService?.setEsimGpio(state)
                oplusEsimService?.setUimPower(1)
                complete(currentGpioState() == state)
            }
        } catch (e: Exception) {
            Log.e(TAG, "Failed to switch eSIM state", e)
            complete(false)
        }
    }

    private fun specialSetEsimGpio(
        state: Int,
        slotMapping: List<UiccSlotMapping>?,
        onComplete: (Boolean) -> Unit,
    ) {
        var seService: SEService? = null

        val listener =
            object : SEService.OnConnectedListener {
                override fun onConnected() {
                    Log.d(TAG, "SEService connected")

                    val service = seService
                    if (service == null) {
                        onComplete(false)
                        return
                    }
                    var success = false

                    try {
                        val reader = service.readers.firstOrNull { it.name == "eSE1" }
                        val session = reader?.openSession()
                        val channel = session?.openLogicalChannel(null)

                        updateModemEsimState(state)
                        updateSimType(state, slotMapping)
                        oplusEsimService?.setEsimGpio(state)
                        oplusEsimService?.setUimPower(1)
                        restoreSimSlotMapping(slotMapping)

                        channel?.close()
                        session?.close()
                        success = currentGpioState() == state
                    } catch (e: Exception) {
                        Log.e(TAG, "Failed to open eSE session", e)
                    } finally {
                        try {
                            service.shutdown()
                        } catch (e: Exception) {
                            Log.e(TAG, "Failed to shutdown SEService", e)
                        }
                        onComplete(success)
                    }
                }
            }

        try {
            seService = SEService(context, Dispatchers.IO.asExecutor(), listener)
        } catch (e: Exception) {
            Log.e(TAG, "Failed to start SEService", e)
            onComplete(false)
        }
    }

    fun restoreEsimState(onComplete: (Boolean) -> Unit) {
        try {
            val service = checkNotNull(oplusEsimService) { "eSIM service is unavailable" }
            val state = service.esimGpio
            check(state == 0 || state == 1) { "Invalid eSIM GPIO state: $state" }
            if (state == 0) {
                onComplete(true)
                return
            }
            val radio =
                ISubsysRadio.Stub.asInterface(
                    ServiceManager.checkService("${ISubsysRadio.DESCRIPTOR}/slot1")
                )
            check(radio != null && radio.interfaceVersion >= 5) {
                "Vendor modem eSIM status API is unavailable"
            }
            Log.i(TAG, "Reinitializing selected eSIM at boot")
            // GPIO may already select eSIM even though the modem failed to initialize it.
            switchEsimState(state, true) { success ->
                if (!success) {
                    try {
                        service.setUimPower(1)
                    } catch (e: Exception) {
                        Log.e(TAG, "Failed to restore UIM power after boot recovery", e)
                    }
                }
                onComplete(success)
            }
        } catch (e: Exception) {
            Log.e(TAG, "Failed to restore eSIM at boot", e)
            onComplete(false)
        }
    }

    private fun updateModemEsimState(state: Int): Boolean {
        try {
            val radio =
                ISubsysRadio.Stub.asInterface(
                    ServiceManager.checkService("${ISubsysRadio.DESCRIPTOR}/slot1")
                )
            if (radio == null || radio.interfaceVersion < 5) {
                Log.w(TAG, "Vendor modem eSIM status API is unavailable")
                return false
            }
            val serial = modemRequestSerial.incrementAndGet()
            // Stock sends this asynchronous feature update before changing the GPIO.
            radio.setEsimStatus(serial, state)
            Log.i(TAG, "Submitted modem eSIM status: state=$state serial=$serial")
            return true
        } catch (e: Exception) {
            Log.e(TAG, "Failed to submit modem eSIM status", e)
            return false
        }
    }

    private fun getSwitchableSlotMapping(): List<UiccSlotMapping>? {
        try {
            val slot =
                context.resources
                    .getIntArray(com.android.internal.R.array.non_removable_euicc_slots)
                    .singleOrNull() ?: return null
            val telephony = context.getSystemService(TelephonyManager::class.java)
            val slotInfo = telephony.uiccSlotsInfo?.getOrNull(slot) ?: return null
            val supportedTypes = slotInfo.supportedSimTypes
            if (
                TelephonyManager.SIM_TYPE_PHYSICAL !in supportedTypes ||
                    TelephonyManager.SIM_TYPE_EMBEDDED !in supportedTypes
            ) {
                Log.d(TAG, "Slot $slot does not advertise SIM type switching")
                return null
            }
            val current = telephony.simSlotMapping.toList()
            check(current.any { it.physicalSlotIndex == slot }) { "eSIM slot is not mapped" }
            return current
        } catch (e: RuntimeException) {
            Log.e(TAG, "Failed to read SIM slot mapping", e)
            return null
        }
    }

    private fun updateSimType(state: Int, slotMapping: List<UiccSlotMapping>?) {
        if (slotMapping == null) return
        try {
            val slot =
                context.resources
                    .getIntArray(com.android.internal.R.array.non_removable_euicc_slots)
                    .single()
            val simType =
                if (state == 1) TelephonyManager.SIM_TYPE_EMBEDDED
                else TelephonyManager.SIM_TYPE_PHYSICAL
            val updated =
                slotMapping.map {
                    if (it.physicalSlotIndex == slot) {
                        UiccSlotMapping(
                            it.portIndex,
                            it.physicalSlotIndex,
                            it.logicalSlotIndex,
                            simType,
                        )
                    } else it
                }
            context.getSystemService(TelephonyManager::class.java).setSimSlotMapping(updated)
        } catch (e: RuntimeException) {
            Log.e(TAG, "Failed to update eSIM slot type", e)
        }
    }

    private fun restoreSimSlotMapping(slotMapping: List<UiccSlotMapping>?) {
        if (slotMapping == null) return
        try {
            // A separate mapping request restores port activation after the SIM type change.
            val mapping =
                slotMapping.map {
                    UiccSlotMapping(it.portIndex, it.physicalSlotIndex, it.logicalSlotIndex)
                }
            context.getSystemService(TelephonyManager::class.java).setSimSlotMapping(mapping)
        } catch (e: RuntimeException) {
            Log.e(TAG, "Failed to restore SIM slot mapping", e)
        }
    }

    companion object {
        private const val TAG = "OplusEsimController"

        private val modemRequestSerial = AtomicInteger(0x45530000)

        private const val NFC_CONFIG_FILE_NAME_PROP = "persist.vendor.nfc.config_file_name"
    }
}
