/*
 * SPDX-FileCopyrightText: 2025-2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher

import android.app.PendingIntent
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.content.pm.PackageManager
import android.os.ServiceManager
import android.os.SystemProperties
import android.se.omapi.SEService
import android.telephony.SubscriptionManager
import android.telephony.TelephonyManager
import android.telephony.UiccSlotMapping
import android.telephony.euicc.EuiccManager
import android.util.Log
import java.util.concurrent.CountDownLatch
import java.util.concurrent.Semaphore
import java.util.concurrent.TimeUnit
import java.util.concurrent.atomic.AtomicInteger
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.asExecutor
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import vendor.oplus.hardware.esim.IOplusEsim
import vendor.oplus.hardware.subsys_interface.subsys_radio.ISubsysRadio

class EsimController(context: Context) {
    private val context = context.applicationContext
    private val hasSN220Chipset = SystemProperties.get(NFC_CONFIG_FILE_NAME_PROP).contains("SN220")

    private val oplusEsimService by lazy {
        IOplusEsim.Stub.asInterface(ServiceManager.getService("${IOplusEsim.DESCRIPTOR}/default"))
    }

    val pendingState: StateFlow<Int?>
        get() = requestedState

    fun currentGpioState() = oplusEsimService?.esimGpio ?: 0

    data class EnableImpact(val physicalSlotIndex: Int, val disablesAllPhysicalSims: Boolean)

    fun getEnableImpact(): EnableImpact? {
        try {
            val slot =
                context.resources
                    .getIntArray(com.android.internal.R.array.non_removable_euicc_slots)
                    .singleOrNull() ?: return null
            if (!hasSN220Chipset || getSwitchableSlotMapping() == null) {
                return EnableImpact(slot, false)
            }
            val profiles = getProfilesToRestore()
            checkProfileCapacity(profiles)
            val modems = context.getSystemService(TelephonyManager::class.java).activeModemCount
            if (profiles.size > 1 && profiles.size < modems) return null
            return EnableImpact(slot, profiles.size > 1)
        } catch (e: RuntimeException) {
            Log.w(TAG, "Unable to determine eSIM enable impact", e)
            return null
        }
    }

    private fun checkProfileCapacity(profiles: List<SavedProfile>) {
        if (profiles.size <= 1) return
        check(context.packageManager.hasSystemFeature(PackageManager.FEATURE_TELEPHONY_EUICC_MEP)) {
            "Multiple enabled eSIM profiles are not supported"
        }
        check(
            profiles.size <= context.getSystemService(TelephonyManager::class.java).activeModemCount
        ) {
            "Not enough active modems for saved eSIM profiles"
        }
        check(
            profiles.map { it.cardId }.distinct().size == 1 &&
                profiles.map { it.portIndex }.distinct().size == profiles.size
        ) {
            "Invalid saved eSIM port assignments"
        }
    }

    fun toggleEsimState(state: Int, onComplete: (Boolean) -> Unit) {
        if (!requestedState.compareAndSet(null, state)) {
            context.mainExecutor.execute { onComplete(false) }
            return
        }
        val complete: (Boolean) -> Unit = { success ->
            requestedState.value = null
            context.mainExecutor.execute { onComplete(success) }
        }
        try {
            checkNotNull(oplusEsimService) { "eSIM service is unavailable" }
            val gpioState = currentGpioState()
            Log.d(TAG, "Current eSIM status = $gpioState")

            if (state == gpioState) {
                Log.d(TAG, "No need to change eSIM state")
                if (
                    state == 1 &&
                        hasSN220Chipset &&
                        profilePreferences().contains(SAVED_PROFILES) &&
                        getSwitchableSlotMapping() != null
                ) {
                    Dispatchers.IO.asExecutor().execute {
                        var success = false
                        try {
                            restoreProfiles()
                            success = true
                        } catch (e: Exception) {
                            Log.e(TAG, "Failed to restore eSIM profiles", e)
                        }
                        complete(success)
                    }
                } else {
                    complete(true)
                }
                return
            }

            val slotMapping = if (hasSN220Chipset) getSwitchableSlotMapping() else null
            if (slotMapping == null) oplusEsimService?.setUimPower(0)

            if (hasSN220Chipset) {
                specialSetEsimGpio(state, slotMapping, complete)
            } else {
                updateModemFeature(state)
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
        Dispatchers.IO.asExecutor().execute {
            var service: SEService? = null
            var success = false
            try {
                if (state == 0 && slotMapping != null) preparePhysicalSimMode(slotMapping)
                val connected = CountDownLatch(1)
                service = SEService(context, Dispatchers.IO.asExecutor()) { connected.countDown() }
                check(connected.await(3, TimeUnit.SECONDS)) { "SEService connection timed out" }
                val reader = service.readers.firstOrNull { it.name == "eSE1" }
                val session = reader?.openSession()
                val channel = session?.openLogicalChannel(null)

                updateModemFeature(state)
                oplusEsimService?.setEsimGpio(state)
                updateSimType(state, slotMapping)

                channel?.close()
                session?.close()
                if (slotMapping != null) awaitSimState(state, slotMapping)
                if (state == 1 && slotMapping != null) restoreProfiles()
                success = currentGpioState() == state
            } catch (e: Exception) {
                Log.e(TAG, "Failed to switch eSIM state", e)
            } finally {
                try {
                    if (slotMapping == null) oplusEsimService?.setUimPower(1)
                } catch (e: Exception) {
                    Log.e(TAG, "Failed to restore UIM power", e)
                    success = false
                }
                try {
                    service?.shutdown()
                } catch (e: Exception) {
                    Log.e(TAG, "Failed to shutdown SEService", e)
                }
                onComplete(success)
            }
        }
    }

    private fun awaitSimState(state: Int, mapping: List<UiccSlotMapping>) {
        val telephony = context.getSystemService(TelephonyManager::class.java)
        val slot =
            context.resources
                .getIntArray(com.android.internal.R.array.non_removable_euicc_slots)
                .single()
        val type =
            if (state == 1) TelephonyManager.SIM_TYPE_EMBEDDED
            else TelephonyManager.SIM_TYPE_PHYSICAL
        val ports =
            mapping
                .map { UiccSlotMapping(it.portIndex, it.physicalSlotIndex, it.logicalSlotIndex) }
                .toSet()
        fun ready(): Boolean {
            val slots = telephony.uiccSlotsInfo ?: return false
            return mapping.all {
                slots.getOrNull(it.physicalSlotIndex)?.simType ==
                    if (it.physicalSlotIndex == slot) type else it.simType
            } &&
                telephony.simSlotMapping
                    .map {
                        UiccSlotMapping(it.portIndex, it.physicalSlotIndex, it.logicalSlotIndex)
                    }
                    .toSet() == ports &&
                telephony.uiccCardsInfo.any {
                    it.physicalSlotIndex == slot && it.isEuicc && (state != 1 || it.cardId >= 0)
                } == (state == 1)
        }
        val changed = Semaphore(0)
        val receiver =
            object : BroadcastReceiver() {
                override fun onReceive(context: Context, intent: Intent) {
                    changed.release()
                }
            }
        context.registerReceiver(
            receiver,
            IntentFilter(TelephonyManager.ACTION_SIM_SLOT_STATUS_CHANGED).apply {
                addAction(TelephonyManager.ACTION_SIM_CARD_STATE_CHANGED)
                addAction(TelephonyManager.ACTION_SIM_APPLICATION_STATE_CHANGED)
            },
            android.Manifest.permission.READ_PRIVILEGED_PHONE_STATE,
            null,
            Context.RECEIVER_EXPORTED,
        )
        try {
            val deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(10)
            while (!ready()) {
                val remaining = deadline - System.nanoTime()
                if (remaining <= 0) break
                // Card and mapping queries can settle after their broadcasts are delivered.
                changed.tryAcquire(
                    minOf(remaining, TimeUnit.MILLISECONDS.toNanos(100)),
                    TimeUnit.NANOSECONDS,
                )
            }
            check(ready()) { "SIM type and port mapping did not settle" }
        } finally {
            context.unregisterReceiver(receiver)
        }
    }

    private fun preparePhysicalSimMode(slotMapping: List<UiccSlotMapping>) {
        val telephony = context.getSystemService(TelephonyManager::class.java)
        val slot =
            context.resources
                .getIntArray(com.android.internal.R.array.non_removable_euicc_slots)
                .single()
        val card =
            telephony.uiccCardsInfo.firstOrNull { it.physicalSlotIndex == slot && it.isEuicc }
        val subscriptions =
            context
                .getSystemService(SubscriptionManager::class.java)
                .activeSubscriptionInfoList
                .orEmpty()
                .filter { it.isEmbedded }
        check(card != null || subscriptions.isEmpty()) { "eUICC card information is unavailable" }
        val activeProfiles = subscriptions.filter { it.cardId == card?.cardId }
        val preferences = profilePreferences()
        if (!preferences.contains(SAVED_PROFILES)) {
            val saved =
                activeProfiles
                    .map { SavedProfile(it.cardId, it.subscriptionId, it.portIndex).encode() }
                    .toSet()
            check(preferences.edit().putStringSet(SAVED_PROFILES, saved).commit()) {
                "Failed to save active eSIM profiles"
            }
        }
        for (subscription in activeProfiles) {
            switchProfile(
                subscription.cardId,
                SubscriptionManager.INVALID_SUBSCRIPTION_ID,
                subscription.portIndex,
            )
        }
        val mapping =
            slotMapping.map {
                UiccSlotMapping(it.portIndex, it.physicalSlotIndex, it.logicalSlotIndex)
            }
        setSlotMapping(mapping)
    }

    private fun setSlotMapping(mapping: List<UiccSlotMapping>) {
        val telephony = context.getSystemService(TelephonyManager::class.java)
        fun mappingMatches() =
            telephony.simSlotMapping
                .map { UiccSlotMapping(it.portIndex, it.physicalSlotIndex, it.logicalSlotIndex) }
                .toSet() == mapping.toSet()
        if (mappingMatches()) return
        val changed = Semaphore(0)
        val receiver =
            object : BroadcastReceiver() {
                override fun onReceive(context: Context, intent: Intent) {
                    changed.release()
                }
            }
        context.registerReceiver(
            receiver,
            IntentFilter(TelephonyManager.ACTION_SIM_SLOT_STATUS_CHANGED),
            android.Manifest.permission.READ_PRIVILEGED_PHONE_STATE,
            null,
            Context.RECEIVER_EXPORTED,
        )
        try {
            telephony.setSimSlotMapping(mapping)
            val deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(10)
            while (!mappingMatches()) {
                val remaining = deadline - System.nanoTime()
                check(remaining > 0) { "Physical SIM mapping did not settle" }
                changed.tryAcquire(remaining, TimeUnit.NANOSECONDS)
            }
        } finally {
            context.unregisterReceiver(receiver)
        }
    }

    private fun profilePreferences() =
        context
            .createDeviceProtectedStorageContext()
            .getSharedPreferences(PROFILE_PREFERENCES, Context.MODE_PRIVATE)

    private fun getProfilesToRestore(): List<SavedProfile> {
        val saved = profilePreferences().getStringSet(SAVED_PROFILES, null) ?: return emptyList()
        if (saved.isEmpty()) return emptyList()
        val available =
            checkNotNull(
                context
                    .getSystemService(SubscriptionManager::class.java)
                    .availableSubscriptionInfoList
            ) {
                "Available eSIM profiles could not be read"
            }
        return saved.map(SavedProfile::decode).filter { profile ->
            available.any {
                it.isEmbedded &&
                    it.cardId == profile.cardId &&
                    it.subscriptionId == profile.subscriptionId
            }
        }
    }

    private fun restoreProfiles() {
        val preferences = profilePreferences()
        if (!preferences.contains(SAVED_PROFILES)) return
        val profiles = getProfilesToRestore()
        checkProfileCapacity(profiles)
        val telephony = context.getSystemService(TelephonyManager::class.java)
        for ((cardId, cardProfiles) in profiles.groupBy { it.cardId }) {
            val card = telephony.uiccCardsInfo.single { it.cardId == cardId && it.isEuicc }
            check(cardProfiles.size <= 1 || card.isMultipleEnabledProfilesSupported) {
                "eUICC does not support multiple enabled profiles"
            }
            check(
                cardProfiles.all { profile -> card.ports.any { it.portIndex == profile.portIndex } }
            ) {
                "Saved eSIM port is unavailable"
            }
        }
        val mapping =
            telephony.simSlotMapping
                .map { UiccSlotMapping(it.portIndex, it.physicalSlotIndex, it.logicalSlotIndex) }
                .toMutableList()
        val ports =
            profiles
                .map { (cardId, _, portIndex) ->
                    val card = telephony.uiccCardsInfo.single { it.cardId == cardId && it.isEuicc }
                    card.physicalSlotIndex to portIndex
                }
                .toSet()
        for ((slot, port) in ports) {
            if (mapping.any { it.physicalSlotIndex == slot && it.portIndex == port }) continue
            // Reuse this eUICC's logical slot before displacing a physical SIM.
            val existing =
                mapping.indexOfFirst {
                    it.physicalSlotIndex == slot && (it.physicalSlotIndex to it.portIndex) !in ports
                }
            val index =
                if (existing >= 0) existing
                else mapping.indexOfFirst { (it.physicalSlotIndex to it.portIndex) !in ports }
            check(index >= 0) { "No logical SIM slot available for saved eSIM profile" }
            mapping[index] = UiccSlotMapping(port, slot, mapping[index].logicalSlotIndex)
        }
        setSlotMapping(mapping)
        for ((cardId, subscriptionId, portIndex) in profiles.sortedBy { it.portIndex }) {
            val active =
                context
                    .getSystemService(SubscriptionManager::class.java)
                    .activeSubscriptionInfoList
                    .orEmpty()
            if (active.any { it.subscriptionId == subscriptionId && it.portIndex == portIndex }) {
                continue
            }
            val euicc = context.getSystemService(EuiccManager::class.java).createForCardId(cardId)
            val deadline = System.nanoTime() + TimeUnit.SECONDS.toNanos(10)
            while (euicc.euiccInfo == null) {
                check(System.nanoTime() < deadline) { "eUICC profile service is not ready" }
                TimeUnit.MILLISECONDS.sleep(100)
            }
            switchProfile(cardId, subscriptionId, portIndex)
        }
        check(preferences.edit().remove(SAVED_PROFILES).commit()) {
            "Failed to clear saved eSIM profiles"
        }
    }

    private fun switchProfile(cardId: Int, subscriptionId: Int, portIndex: Int) {
        val euicc = context.getSystemService(EuiccManager::class.java).createForCardId(cardId)
        val action = "${context.packageName}.SWITCH_PROFILE.${modemRequestSerial.incrementAndGet()}"
        val finished = CountDownLatch(1)
        var result = EuiccManager.EMBEDDED_SUBSCRIPTION_RESULT_ERROR
        val receiver =
            object : BroadcastReceiver() {
                override fun onReceive(context: Context, intent: Intent) {
                    result = resultCode
                    finished.countDown()
                }
            }
        context.registerReceiver(receiver, IntentFilter(action), Context.RECEIVER_NOT_EXPORTED)
        val callback =
            PendingIntent.getBroadcast(
                context,
                0,
                Intent(action).setPackage(context.packageName),
                PendingIntent.FLAG_UPDATE_CURRENT or PendingIntent.FLAG_MUTABLE,
            )
        try {
            euicc.switchToSubscription(subscriptionId, portIndex, callback)
            check(finished.await(30, TimeUnit.SECONDS)) { "Profile switch timed out" }
            check(result == EuiccManager.EMBEDDED_SUBSCRIPTION_RESULT_OK) {
                "Profile switch failed: $result"
            }
        } finally {
            callback.cancel()
            context.unregisterReceiver(receiver)
        }
    }

    private fun getSwitchableSlotMapping(): List<UiccSlotMapping>? {
        try {
            val slot =
                context.resources
                    .getIntArray(com.android.internal.R.array.non_removable_euicc_slots)
                    .singleOrNull() ?: return null
            val telephony = context.getSystemService(TelephonyManager::class.java)
            val slots = checkNotNull(telephony.uiccSlotsInfo) { "SIM slots are unavailable" }
            val slotInfo = checkNotNull(slots.getOrNull(slot)) { "eSIM slot is unavailable" }
            val supportedTypes = slotInfo.supportedSimTypes
            if (
                TelephonyManager.SIM_TYPE_PHYSICAL !in supportedTypes ||
                    TelephonyManager.SIM_TYPE_EMBEDDED !in supportedTypes
            ) {
                Log.d(TAG, "Slot $slot does not advertise SIM type switching")
                return null
            }
            // SIM-type requests must describe every physical slot, including inactive ones.
            return slots.mapIndexed { index, info ->
                UiccSlotMapping(0, index, index, checkNotNull(info).simType)
            }
        } catch (e: RuntimeException) {
            throw IllegalStateException("Failed to read SIM slot types", e)
        }
    }

    private fun updateModemFeature(state: Int) {
        try {
            val radio =
                ISubsysRadio.Stub.asInterface(
                    ServiceManager.checkService("${ISubsysRadio.DESCRIPTOR}/slot1")
                )
            if (radio == null || radio.interfaceVersion < ISubsysRadio.VERSION) {
                Log.w(TAG, "Vendor modem eSIM status API is unavailable")
                return
            }
            val serial = modemRequestSerial.incrementAndGet()
            // Stock sends this asynchronous feature update before changing the GPIO.
            radio.setEsimStatus(serial, state)
            Log.i(TAG, "Submitted modem eSIM status: state=$state serial=$serial")
        } catch (e: Exception) {
            Log.e(TAG, "Failed to submit modem eSIM status", e)
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
            throw IllegalStateException("Failed to update eSIM slot type", e)
        }
    }

    private data class SavedProfile(val cardId: Int, val subscriptionId: Int, val portIndex: Int) {
        init {
            require(
                cardId >= 0 &&
                    SubscriptionManager.isValidSubscriptionId(subscriptionId) &&
                    portIndex >= 0
            ) {
                "Invalid saved eSIM profile"
            }
        }

        fun encode() = "$cardId:$subscriptionId:$portIndex"

        companion object {
            fun decode(value: String): SavedProfile {
                val fields = value.split(':')
                require(fields.size == 3) { "Invalid saved eSIM profile" }
                return SavedProfile(fields[0].toInt(), fields[1].toInt(), fields[2].toInt())
            }
        }
    }

    companion object {
        private const val TAG = "OplusEsimController"
        private const val PROFILE_PREFERENCES = "esim_profiles"
        private const val SAVED_PROFILES = "profiles"

        private val requestedState = MutableStateFlow<Int?>(null)

        private val modemRequestSerial = AtomicInteger(0x45530000)

        private const val NFC_CONFIG_FILE_NAME_PROP = "persist.vendor.nfc.config_file_name"
    }
}
