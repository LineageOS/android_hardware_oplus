/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.memc

import android.app.ActivityTaskManager
import android.app.KeyguardManager
import android.app.Service
import android.app.TaskStackListener
import android.app.WindowConfiguration
import android.content.BroadcastReceiver
import android.content.ComponentName
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.content.pm.PackageManager
import android.database.ContentObserver
import android.hardware.display.DisplayManager
import android.media.AudioAttributes
import android.media.AudioManager
import android.media.AudioPlaybackConfiguration
import android.os.Handler
import android.os.IBinder
import android.os.Looper
import android.os.PowerManager
import android.os.Process
import android.os.RemoteException
import android.os.ServiceManager
import android.os.ServiceSpecificException
import android.provider.Settings
import android.util.Log
import android.util.Size
import android.view.Display
import android.view.Surface
import android.widget.Toast
import vendor.pixelworks.hardware.display.ContentSamples
import vendor.pixelworks.hardware.display.IIris
import vendor.pixelworks.hardware.display.IIrisCallback

class MemcService : Service() {
    private val handler = Handler(Looper.getMainLooper())

    private lateinit var audioManager: AudioManager
    private lateinit var displayManager: DisplayManager
    private lateinit var keyguardManager: KeyguardManager
    private lateinit var powerManager: PowerManager

    private var excludedApps = emptySet<String>()
    private var focusedActivity: ComponentName? = null
    private var focusedUid = Process.INVALID_UID
    private var focusedFullscreen = false
    private var playbackConfigs = emptyList<AudioPlaybackConfiguration>()
    private var active = false
    private var activeSize: Size? = null
    private var entered = false
    private var announced = false
    private var irisMemcOn = false

    private var boostTo120 = false

    private val enterRunnable = Runnable { enter() }
    private val announceRunnable = Runnable { onMemcStarted() }

    private val irisCallback =
        object : IIrisCallback.Stub() {
            override fun onFeatureChanged(feature: Int, params: IntArray) {
                val on = params.firstOrNull()?.let { it != 0 } ?: false
                handler.post { onIrisMemcChanged(on) }
            }

            override fun onRefreshRequested(timestamp: Long) {}

            override fun onCalibratePatternChanged(timestamp: Long, pattern: Int) = 0

            override fun onDisplayPowerChanged(timestamp: Long, power: Int) {}

            override fun onContentSamplingRequested(timestamp: Long, layer: Int, duration: Long) =
                ContentSamples().apply { result = -1 }

            override fun getInterfaceHash(): String = IIrisCallback.HASH

            override fun getInterfaceVersion(): Int = IIrisCallback.VERSION
        }

    private val taskStackListener =
        object : TaskStackListener() {
            override fun onTaskStackChanged() {
                handler.post { updateFocusedActivity() }
            }
        }

    private val displayListener =
        object : DisplayManager.DisplayListener {
            override fun onDisplayAdded(displayId: Int) {}

            override fun onDisplayRemoved(displayId: Int) {}

            override fun onDisplayChanged(displayId: Int) {
                if (displayId == Display.DEFAULT_DISPLAY) {
                    update()
                }
            }
        }

    private val playbackCallback =
        object : AudioManager.AudioPlaybackCallback() {
            override fun onPlaybackConfigChanged(configs: List<AudioPlaybackConfiguration>) {
                playbackConfigs = configs
                update()
            }
        }

    private val excludedAppsObserver =
        object : ContentObserver(handler) {
            override fun onChange(selfChange: Boolean) {
                excludedApps = Utils.getExcludedApps(this@MemcService)
                update()
            }
        }

    private val stateReceiver =
        object : BroadcastReceiver() {
            override fun onReceive(context: Context, intent: Intent) {
                update()
            }
        }

    private val thermalListener = PowerManager.OnThermalStatusChangedListener { update() }

    override fun onCreate() {
        Log.d(TAG, "Creating service")

        audioManager = getSystemService(AudioManager::class.java)!!
        displayManager = getSystemService(DisplayManager::class.java)!!
        keyguardManager = getSystemService(KeyguardManager::class.java)!!
        powerManager = getSystemService(PowerManager::class.java)!!

        excludedApps = Utils.getExcludedApps(this)
        contentResolver.registerContentObserver(
            Settings.Secure.getUriFor(Utils.MEMC_EXCLUDED_APPS),
            false,
            excludedAppsObserver,
        )
        ActivityTaskManager.getService().registerTaskStackListener(taskStackListener)
        displayManager.registerDisplayListener(
            displayListener,
            handler,
            DisplayManager.EVENT_TYPE_DISPLAY_CHANGED or DisplayManager.EVENT_TYPE_DISPLAY_STATE,
        )
        audioManager.registerAudioPlaybackCallback(playbackCallback, handler)
        registerReceiver(
            stateReceiver,
            IntentFilter().apply {
                addAction(Intent.ACTION_USER_PRESENT)
                addAction(PowerManager.ACTION_POWER_SAVE_MODE_CHANGED)
            },
        )
        powerManager.addThermalStatusListener(mainExecutor, thermalListener)
        try {
            getIris()?.registerCallback(Process.myPid().toLong(), irisCallback)
        } catch (e: RemoteException) {
            Log.e(TAG, "Failed to register Iris callback", e)
        }

        playbackConfigs = audioManager.activePlaybackConfigurations
        updateFocusedActivity()
    }

    override fun onDestroy() {
        Log.d(TAG, "Destroying service")

        contentResolver.unregisterContentObserver(excludedAppsObserver)
        ActivityTaskManager.getService().unregisterTaskStackListener(taskStackListener)
        displayManager.unregisterDisplayListener(displayListener)
        audioManager.unregisterAudioPlaybackCallback(playbackCallback)
        unregisterReceiver(stateReceiver)
        powerManager.removeThermalStatusListener(thermalListener)

        handler.removeCallbacks(enterRunnable)
        exit()
        displayManager.requestMaxRefreshRate(Display.DEFAULT_DISPLAY, 0f)
    }

    override fun onBind(intent: Intent?): IBinder? = null

    private fun updateFocusedActivity() {
        val task = ActivityTaskManager.getService().focusedRootTaskInfo
        if (task?.topActivity != focusedActivity) {
            focusedActivity = task?.topActivity
            announced = false
        }
        focusedUid =
            task?.topActivity?.let { getPackageUid(it.packageName, task.userId) }
                ?: Process.INVALID_UID
        focusedFullscreen = task?.windowingMode == WindowConfiguration.WINDOWING_MODE_FULLSCREEN
        update()
    }

    private fun getPackageUid(packageName: String, userId: Int) =
        try {
            packageManager.getPackageUidAsUser(packageName, userId)
        } catch (e: PackageManager.NameNotFoundException) {
            Process.INVALID_UID
        }

    // Iris only interpolates video layers, so any media playback is enough.
    private fun isFocusedActivityPlaying() =
        playbackConfigs.any {
            it.clientUid == focusedUid &&
                it.playerState == AudioPlaybackConfiguration.PLAYER_STATE_STARTED &&
                it.audioAttributes.usage == AudioAttributes.USAGE_MEDIA
        }

    private fun update() {
        val display = displayManager.getDisplay(Display.DEFAULT_DISPLAY)
        val landscape =
            display.rotation == Surface.ROTATION_90 || display.rotation == Surface.ROTATION_270
        val shouldBeActive =
            focusedFullscreen &&
                focusedActivity?.packageName !in excludedApps &&
                isFocusedActivityPlaying() &&
                landscape &&
                display.state == Display.STATE_ON &&
                !keyguardManager.isKeyguardLocked &&
                !powerManager.isPowerSaveMode &&
                powerManager.currentThermalStatus < PowerManager.THERMAL_STATUS_SEVERE
        val size = Size(display.mode.physicalWidth, display.mode.physicalHeight)

        // Iris switches to bypass on resolution changes, so enter again.
        if (active && (!shouldBeActive || size != activeSize)) {
            active = false
            handler.removeCallbacks(enterRunnable)
            exit()
            displayManager.requestMaxRefreshRate(Display.DEFAULT_DISPLAY, 0f)
        }

        if (!active && shouldBeActive) {
            active = true
            activeSize = size
            boostTo120 = Utils.getMemcMode(this) == Utils.MEMC_MODE_120
            displayManager.requestMaxRefreshRate(
                Display.DEFAULT_DISPLAY,
                if (boostTo120) MEMC_REFRESH_RATE_120 else MEMC_REFRESH_RATE_60,
            )
            handler.postDelayed(enterRunnable, ENTER_DELAY_MS)
        }
    }

    private fun enter() {
        val iris = getIris() ?: return
        try {
            if (iris.getConfig(IRIS_MODE) == IRIS_MODE_BYPASS) {
                iris.irisConfigureSet(IRIS_MODE, intArrayOf(IRIS_MODE_PASS_THROUGH))
            }
            iris.irisConfigureSet(
                IRIS_MEMC,
                intArrayOf(
                    IRIS_MEMC_SINGLE,
                    IRIS_MEMC_IGNORED,
                    IRIS_MEMC_LEVEL shl IRIS_MEMC_LEVEL_SHIFT,
                ),
            )
            irisMemcOn = false
            entered = true
            Log.d(TAG, "Entered MEMC")
        } catch (e: RemoteException) {
            Log.e(TAG, "Failed to enter MEMC", e)
        } catch (e: ServiceSpecificException) {
            Log.e(TAG, "Failed to enter MEMC", e)
        }
    }

    // Iris briefly enters MEMC while players start, so only announce it once it stays on.
    private fun onIrisMemcChanged(on: Boolean) {
        if (!entered || on == irisMemcOn) {
            return
        }

        irisMemcOn = on
        handler.removeCallbacks(announceRunnable)
        if (on) {
            handler.postDelayed(announceRunnable, ANNOUNCE_DELAY_MS)
        }
    }

    private fun onMemcStarted() {
        val packageName = focusedActivity?.packageName
        if (!entered || packageName == null) {
            return
        }

        Utils.addEnhancedApp(this, packageName)
        if (!announced) {
            announced = true
            Toast.makeText(
                    this,
                    if (boostTo120) R.string.memc_starting_120 else R.string.memc_starting,
                    Toast.LENGTH_SHORT,
                )
                .show()
        }
    }

    private fun exit() {
        if (!entered) {
            return
        }

        entered = false
        irisMemcOn = false
        handler.removeCallbacks(announceRunnable)
        val iris = getIris() ?: return
        try {
            if (iris.getConfig(IRIS_MEMC) != IRIS_MEMC_OFF) {
                iris.irisConfigureSet(IRIS_MEMC, intArrayOf(IRIS_MEMC_OFF))
            }
            iris.irisConfigureSet(IRIS_MODE, intArrayOf(IRIS_MODE_BYPASS))
            Log.d(TAG, "Exited MEMC")
        } catch (e: RemoteException) {
            Log.e(TAG, "Failed to exit MEMC", e)
        } catch (e: ServiceSpecificException) {
            Log.e(TAG, "Failed to exit MEMC", e)
        }
    }

    private fun getIris(): IIris? {
        val binder = ServiceManager.checkService(IRIS_SERVICE)
        if (binder == null) {
            Log.e(TAG, "Iris service is unavailable")
            return null
        }
        return IIris.Stub.asInterface(binder)
    }

    // Like stock, a failed query reads as an unknown, nonzero value.
    private fun IIris.getConfig(id: Int): Int? =
        try {
            irisConfigureGet(id, intArrayOf(0)).firstOrNull()
        } catch (e: RemoteException) {
            Log.w(TAG, "Failed to get Iris config $id", e)
            null
        } catch (e: ServiceSpecificException) {
            Log.w(TAG, "Failed to get Iris config $id", e)
            null
        }

    companion object {
        private const val TAG = "OplusMemc"

        private const val ENTER_DELAY_MS = 600L
        private const val ANNOUNCE_DELAY_MS = 2000L
        private const val MEMC_REFRESH_RATE_60 = 60f
        private const val MEMC_REFRESH_RATE_120 = 120f

        private val IRIS_SERVICE = "${IIris.DESCRIPTOR}/default"

        private const val IRIS_MODE = 56
        private const val IRIS_MODE_PASS_THROUGH = 0
        private const val IRIS_MODE_BYPASS = 1
        private const val IRIS_MEMC = 258
        private const val IRIS_MEMC_OFF = 0
        private const val IRIS_MEMC_SINGLE = 10
        private const val IRIS_MEMC_IGNORED = -1
        // Normal video scene, the level is a signed 4-bit value.
        private const val IRIS_MEMC_LEVEL = 7
        private const val IRIS_MEMC_LEVEL_SHIFT = 8
    }
}
