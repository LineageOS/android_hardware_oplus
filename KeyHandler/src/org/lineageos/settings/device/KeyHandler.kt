/*
 * Copyright (C) 2021-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device

import android.app.NotificationManager
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.media.AudioManager
import android.media.AudioSystem
import android.os.VibrationAttributes
import android.os.VibrationEffect
import android.os.Vibrator
import android.provider.Settings
import android.view.KeyEvent
import com.android.internal.os.DeviceKeyHandler
import java.io.File
import java.util.concurrent.Executors
import android.service.notification.Condition
import android.net.Uri
import android.util.Log

class KeyHandler(context: Context) : DeviceKeyHandler {
    private val audioManager = context.getSystemService(AudioManager::class.java)!!
    private val notificationManager = context.getSystemService(NotificationManager::class.java)!!
    private val vibrator = context.getSystemService(Vibrator::class.java)!!

    private val packageContext =
        context.createPackageContext(KeyHandler::class.java.getPackage()!!.name, 0)
    private val sharedPreferences
        get() =
            packageContext.getSharedPreferences(
                packageContext.packageName + "_preferences",
                Context.MODE_PRIVATE or Context.MODE_MULTI_PROCESS,
            )

    private val executorService = Executors.newSingleThreadExecutor()


    private sealed class Previous {
        class DndRule(val ruleId: String) : Previous()
        class AudioManaged(val ringerMode: Int) : Previous()
        class ZenMode(val mode: Int) : Previous()
    }

    private var previousState: Previous = Previous.AudioManaged(0);

    private var wasMuted = false
    private val broadcastReceiver =
        object : BroadcastReceiver() {
            override fun onReceive(context: Context, intent: Intent) {
                when (intent.action) {
                    AudioManager.STREAM_MUTE_CHANGED_ACTION -> {
                        val stream = intent.getIntExtra(AudioManager.EXTRA_VOLUME_STREAM_TYPE, -1)
                        val state =
                            intent.getBooleanExtra(AudioManager.EXTRA_STREAM_VOLUME_MUTED, false)
                        if (stream == AudioSystem.STREAM_MUSIC && !state) {
                            wasMuted = false
                        }
                    }

                    Intent.ACTION_BOOT_COMPLETED -> {
                        Log.d(TAG, "Boot completed - populating key state")
                        populateKeyState(false)
                    }
                    // TODO listen for ButtonSettingsFragmentA changes?

                }
            }
        }

    init {
        Log.d(TAG, "Registering broadcast receiver")
        context.registerReceiver(
            broadcastReceiver,
            IntentFilter().apply {
                addAction(AudioManager.STREAM_MUTE_CHANGED_ACTION)
                addAction(Intent.ACTION_BOOT_COMPLETED)
            },
        )
    }

    override fun handleKeyEvent(event: KeyEvent): KeyEvent? {
        if (event.action != KeyEvent.ACTION_DOWN) {
            return event
        }

        val deviceName = event.device.name

        if (deviceName != "oplus,hall_tri_state_key" && deviceName != "oplus,tri-state-key") {
            return event
        }

        populateKeyState(true)

        return null
    }

    private fun populateKeyState(vibrate: Boolean) {
        when (File("/proc/tristatekey/tri_state").readText().trim()) {
            "1" -> handleMode(POSITION_TOP, vibrate)
            "2" -> handleMode(POSITION_MIDDLE, vibrate)
            "3" -> handleMode(POSITION_BOTTOM, vibrate)
            else -> return
        }
    }

    private fun vibrateIfNeeded(mode: Int) {
        when (mode) {
            AudioManager.RINGER_MODE_VIBRATE ->
                vibrator.vibrate(MODE_VIBRATION_EFFECT, HARDWARE_FEEDBACK_VIBRATION_ATTRIBUTES)
            AudioManager.RINGER_MODE_NORMAL ->
                vibrator.vibrate(MODE_NORMAL_EFFECT, HARDWARE_FEEDBACK_VIBRATION_ATTRIBUTES)
        }
    }

    private fun handleMode(position: Int, vibrate: Boolean) {
        Log.d(TAG, "Previous state: $previousState")
        when (previousState) {
            is Previous.AudioManaged -> {
                Log.d(TAG, "Previous state was AudioManaged: state=${(previousState as Previous.AudioManaged).ringerMode}")
            }
            is Previous.ZenMode -> {
                Log.d(TAG, "Previous state was ZenMode: mode=${(previousState as Previous.ZenMode).mode}")
            }
            is Previous.DndRule -> {
                Log.d(TAG, "Previous state was DndRule: ruleId=${(previousState as Previous.DndRule).ruleId}")
            }
            else -> {
                Log.d(TAG, "No previous state recorded. Initializing to AudioManaged with current ringer mode.")
                previousState = Previous.AudioManaged(audioManager.ringerModeInternal)
            }
        }
        val actionKey = when (position) {
            POSITION_TOP -> ALERT_SLIDER_TOP_KEY
            POSITION_MIDDLE -> ALERT_SLIDER_MIDDLE_KEY
            POSITION_BOTTOM -> ALERT_SLIDER_BOTTOM_KEY
            else -> return
        }

        val dndKey = when (position) {
            POSITION_TOP -> ALERT_SLIDER_TOP_DND_KEY
            POSITION_MIDDLE -> ALERT_SLIDER_MIDDLE_DND_KEY
            POSITION_BOTTOM -> ALERT_SLIDER_BOTTOM_DND_KEY
            else -> return
        }

        // Action selected by the user (as a string)
        val actionString = sharedPreferences.getString(actionKey, "0")
        Log.d(TAG, "Handling position $position with action $actionString: $dndKey $actionKey")

        val mode = actionString!!.toIntOrNull() ?: return // Convert to Int, use default if failed

        executorService.submit {
            undoPrevious() // first undo previous state
            when (mode) {
                AudioManager.RINGER_MODE_SILENT -> {
                    // clear dnd rule if it is set by us
                    //setZenMode(Settings.Global.ZEN_MODE_OFF) // don't assume user wants to clear DND when going to silent: let the undoPrevious() handle it
                    setAMRingerModeTo(mode);
                    if (sharedPreferences.getBoolean(MUTE_MEDIA_WITH_SILENT, false)) {
                        audioManager.adjustVolume(AudioManager.ADJUST_MUTE, 0)
                        wasMuted = true
                    }
                    Log.d(TAG, "(audio mode silent) Set previousState to AudioManaged with mode: $mode");
                    previousState = Previous.AudioManaged(mode)
                    Log.d(TAG, "(audio mode silent) Updated ringer mode for position $position, previousState=$previousState");
                }
                AudioManager.RINGER_MODE_VIBRATE,
                AudioManager.RINGER_MODE_NORMAL -> {
                    // clear dnd rule if it is set by us
                    //setZenMode(Settings.Global.ZEN_MODE_OFF) // don't assume user wants to clear DND when going to vibrate/sound: let the undoPrevious() handle it
                    setAMRingerModeTo(mode);
                    if (sharedPreferences.getBoolean(MUTE_MEDIA_WITH_SILENT, false) && wasMuted) {
                        audioManager.adjustVolume(AudioManager.ADJUST_UNMUTE, 0)
                    }
                    Log.d(TAG, "(audio mode not silent) Set previousState to AudioManaged with mode: $mode");
                    previousState = Previous.AudioManaged(mode)
                    Log.d(TAG, "(audio mode not silent)Updated ringer mode for position $position, previousState=$previousState");
                }
                ZEN_PRIORITY_ONLY,
                ZEN_TOTAL_SILENCE,
                ZEN_ALARMS_ONLY -> {
                    //audioManager.ringerModeInternal = AudioManager.RINGER_MODE_NORMAL // disabled because we want the default notification mode (vibrate/sound/silent as set by the user) to persist when activating a zen mode
                    setZenMode(mode - ZEN_OFFSET)
                    if (sharedPreferences.getBoolean(MUTE_MEDIA_WITH_SILENT, false) && wasMuted) {
                        audioManager.adjustVolume(AudioManager.ADJUST_UNMUTE, 0)
                    }
                    if (mode == ZEN_PRIORITY_ONLY || mode == ZEN_ALARMS_ONLY) {
                        updateRingerMode(position)
                    }
                    Log.d(TAG, "(zen mode) Updated ringer mode to  for position $position, previousState=$previousState");
                    previousState = Previous.ZenMode(mode)
                    Log.d(TAG, "(zen mode) Set previousState to ZenMode with mode: ${(previousState as Previous.ZenMode).mode}");
                }
                ACTION_DND_MODE -> {
                    // --- UPDATED DND MODE HANDLING ---
                    // do not change ringer mode, just activate the selected DND rule - we want to preserve the user's current ringer mode (vibrate/sound/silent)
                    Log.d(TAG, "Handling custom DND action for key $dndKey ($actionKey)");
                    handleDndAction(dndKey);
                    updateRingerMode(position);
                    Log.d(TAG, "(dnd action) Updated ringer mode for position $position, previousState=$previousState");
                    previousState = Previous.DndRule(sharedPreferences.getString(dndKey, "") ?: "")
                    Log.d(TAG, "(dnd action) Set previousState to DndRule with ID: ${(previousState as Previous.DndRule).ruleId}");
                }
            }

            if (vibrate && mode != ACTION_DND_MODE) {
                vibrateIfNeeded(mode)
            }
        }
    }
    private fun updateRingerMode(position: Int) {
        Log.d(TAG, "(updateRingerMode) Updating ringer mode for position $position");
        val ringerKey = when (position) {
            POSITION_TOP -> KEY_POSITION_TOP_RINGER
            POSITION_MIDDLE -> KEY_POSITION_MIDDLE_RINGER
            POSITION_BOTTOM -> KEY_POSITION_BOTTOM_RINGER
            else -> KEY_POSITION_MIDDLE_RINGER
        }
        Log.d(TAG, "(updateRingerMode) Using ringerKey: $ringerKey");
        Log.d(TAG, "(updateRingerMode) SharedPreferences contains keys: " + sharedPreferences.all.keys.toString());
        val ringerMode = sharedPreferences.getString(
            ringerKey,
            AudioManager.RINGER_MODE_NORMAL.toString(),
        )!!.toInt()
	    Log.d(TAG, "(updateRingerMode) ringerMode=$ringerMode");
        setAMRingerModeTo(ringerMode);
        Log.d(TAG, "(updateRingerMode) Updated ringer mode to $ringerMode for position $position")
        // this is technically not what the user's direct intent is so we don't set previousState here
        // we do need to mute media if this is silent mode though if applicable
        // and take it off mute when going back to sound/vibrate
        if (ringerMode == AudioManager.RINGER_MODE_SILENT && sharedPreferences.getBoolean(MUTE_MEDIA_WITH_SILENT, false)) {
            audioManager.adjustVolume(AudioManager.ADJUST_MUTE, 0)
            wasMuted = true
        } else if (sharedPreferences.getBoolean(MUTE_MEDIA_WITH_SILENT, false) && wasMuted) {
            audioManager.adjustVolume(AudioManager.ADJUST_UNMUTE, 0)
        }
        Log.d(TAG, "(updateRingerMode) wasMuted state is now $wasMuted, previousState=$previousState")
    }

    private fun setAMRingerModeTo(ringerMode: Int) {
        while (audioManager.ringerModeInternal != ringerMode) {
            audioManager.ringerModeInternal = ringerMode
            Thread.sleep(50) // small delay to allow the system to process the change
        }
    }

    private fun undoPrevious() {
        // NOTE: Setting ZEN_MODE_OFF will automatically deactivate all active DND rules...
        // we may need to re-activate other rules if we want to keep them active (which we do).
        when (previousState) {
            is Previous.AudioManaged -> {
                // do nothing here because the ringer mode is already set, it will simply be updated
            }
            is Previous.ZenMode -> {
                Log.d(TAG, "Resetting previous Zen Mode to OFF");
                val rules = notificationManager.getAutomaticZenRules(); // Map<String, AutomaticZenRule>
                val activeRules = mutableListOf<String>();
                for (id in rules.keys) {
                    if (notificationManager.getAutomaticZenRuleState(id) == Condition.STATE_TRUE) {
                        activeRules.add(id);
                    }
                }
                setZenMode(Settings.Global.ZEN_MODE_OFF);
                // re-activate other active rules
                for (id in activeRules) {
                    Log.d(TAG, "Re-activating previously active DND rule with ID: $id")
                    notificationManager.setAutomaticZenRuleState(
                        id,
                        Condition(
                        Uri.Builder()
                            .scheme("content")
                            .authority("org.lineageos.settings.device")
                            .build(),
                        "Tri-State Key position reactivation after Zen Mode OFF",
                        Condition.STATE_TRUE,
                        Condition.SOURCE_USER_ACTION
                        )
                    )
                }
            }
            is Previous.DndRule -> {
                val previousRuleId = (previousState as Previous.DndRule).ruleId
                if (previousRuleId.isNotEmpty()) {
                    Log.d(TAG, "Deactivating previous DND rule with ID: $previousRuleId")
                    notificationManager.setAutomaticZenRuleState(
                        previousRuleId,
                        Condition(
                        Uri.Builder()
                            .scheme("content")
                            .authority("org.lineageos.settings.device")
                            .build(),
                        "Tri-State Key position",
                        Condition.STATE_FALSE,
                        Condition.SOURCE_USER_ACTION
                        )
                    ) 
                }
            }
        }
    }

    /**
     * Handles the activation of a user-selected DND/Zen mode rule.
     * This activates a rule and allows other active rules to remain active.
     */
    private fun handleDndAction(dndKey: String) {
        // 1. Get the ID of the user-selected Zen Rule.
        val selectedZenRuleId = sharedPreferences.getString(dndKey, null) ?: return
        Log.d(TAG, "Activating DND rule with ID ($dndKey): $selectedZenRuleId")
        // 2. Ensure the global ringer mode is set to NORMAL for DND rules to apply.
        //audioManager.ringerModeInternal = AudioManager.RINGER_MODE_NORMAL // disabled because we want the default notification mode (vibrate/sound/silent as set by the user) to persist when activating a DND rule

        // 4. Activate the user-selected DND rule using the correct API.
        //    This method is often used by system UIs to toggle Zen rule state.
        notificationManager.setAutomaticZenRuleState(
            selectedZenRuleId,
            Condition(
            Uri.Builder()
                .scheme("content")
                .authority("org.lineageos.settings.device")
                .build(),
            "Tri-State Key position",
            Condition.STATE_TRUE,
            Condition.SOURCE_USER_ACTION
            )
        )
        Log.d(TAG, "Activated DND rule with ID: $selectedZenRuleId")
        Log.d(TAG, "rule state: " + notificationManager.getAutomaticZenRuleState(selectedZenRuleId).toString());
        
    }

    private fun setZenMode(zenMode: Int) {
        // Set zen mode
        notificationManager.setZenMode(zenMode, null, TAG)

        // Wait until zen mode change is committed
        while (notificationManager.zenMode != zenMode) {
            Thread.sleep(75)
        }
    }

    companion object {
        private const val TAG = "KeyHandler"

        // Slider key positions
        private const val POSITION_TOP = 1
        private const val POSITION_MIDDLE = 2
        private const val POSITION_BOTTOM = 3

        // Action value for the user-selected DND/Zen rule
        private const val ACTION_DND_MODE = 10 // Matches VALUE_DND_MODE_ACTION = "10"

        // Preference keys (Action)
        private const val ALERT_SLIDER_TOP_KEY = "config_top_position"
        private const val ALERT_SLIDER_MIDDLE_KEY = "config_middle_position"
        private const val ALERT_SLIDER_BOTTOM_KEY = "config_bottom_position"
        private const val MUTE_MEDIA_WITH_SILENT = "config_mute_media"

        // Preference keys (Dependent DND/Zen Mode)
        private const val ALERT_SLIDER_TOP_DND_KEY = "config_top_zen_mode"
        private const val ALERT_SLIDER_MIDDLE_DND_KEY = "config_middle_zen_mode"
        private const val ALERT_SLIDER_BOTTOM_DND_KEY = "config_bottom_zen_mode"

        // Preference keys (Ringer Mode for Zen Priority Only, Alarms Only and DND Modes)
        private const val KEY_POSITION_TOP_RINGER = "config_top_ringer_mode"
        private const val KEY_POSITION_MIDDLE_RINGER = "config_middle_ringer_mode"
        private const val KEY_POSITION_BOTTOM_RINGER = "config_bottom_ringer_mode"

        // ZEN constants (Existing built-in modes)
        private const val ZEN_OFFSET = 2
        private const val ZEN_PRIORITY_ONLY = 3
        private const val ZEN_TOTAL_SILENCE = 4
        private const val ZEN_ALARMS_ONLY = 5

        // Vibration attributes
        private val HARDWARE_FEEDBACK_VIBRATION_ATTRIBUTES =
            VibrationAttributes.createForUsage(VibrationAttributes.USAGE_HARDWARE_FEEDBACK)

        // Vibration effects
        private val MODE_NORMAL_EFFECT = VibrationEffect.get(VibrationEffect.EFFECT_HEAVY_CLICK)
        private val MODE_VIBRATION_EFFECT = VibrationEffect.get(VibrationEffect.EFFECT_DOUBLE_CLICK)
    }
}
