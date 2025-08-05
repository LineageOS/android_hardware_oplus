/*
 * SPDX-FileCopyrightText: 2022-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.ifaa.aidl.manager

import android.app.KeyguardManager
import android.app.Service
import android.content.Intent
import android.graphics.Point
import android.hardware.fingerprint.FingerprintManager
import android.os.IBinder
import android.os.ServiceManager
import android.os.SystemProperties
import android.provider.Settings
import android.util.Log
import android.view.WindowManager
import org.json.JSONObject
import vendor.oplus.hardware.biometrics.fingerprintpay.IFingerprintPay

class IfaaService : Service() {
    private var _fpPayService: IFingerprintPay? = null

    private val fpPayServiceDeathRecipient = IBinder.DeathRecipient {
        Log.i(LOG_TAG, "aidl FingerprintPay hal died, reset hal proxy!")
        _fpPayService = null
    }

    private val fingerprintManager by lazy { getSystemService(FingerprintManager::class.java)!! }
    private val keyguardManager by lazy { getSystemService(KeyguardManager::class.java)!! }
    private val windowManager by lazy { getSystemService(WindowManager::class.java)!! }

    override fun onBind(intent: Intent) = object : IfaaManagerService.Stub() {
        private val _supportBIOTypes by lazy {
            when (SystemProperties.get(FP_TYPE_PROP, "")) {
                "back", "side", "front" -> AUTH_TYPE_FINGERPRINT
                "ultrasonic", "optical" -> AUTH_TYPE_OPTICAL_FINGERPRINT
                else -> AUTH_TYPE_NOT_SUPPORT
            }
        }

        override fun getSupportBIOTypes() = _supportBIOTypes

        override fun startBIOManager(authType: Int) = when (authType) {
            AUTH_TYPE_FINGERPRINT -> {
                applicationContext.startActivity(
                    Intent(Settings.ACTION_SECURITY_SETTINGS).apply {
                        addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
                    }
                )

                COMMAND_OK
            }

            else -> COMMAND_FAIL
        }

        private val _deviceModel by lazy { SystemProperties.get(IFAA_MODEL_PROP, "OPLUS-Default") }
        override fun getDeviceModel() = _deviceModel

        override fun processCmd(param: ByteArray) = try {
            getFpPayService()?.ifaa_invoke_command(param)
        } catch (e: Exception) {
            Log.e(LOG_TAG, "processCmdImpl: ifaa_invoke_command aidl failed", e)
            null
        }

        override fun getVersion() = 4

        override fun getExtInfo(authType: Int, keyExtInfo: String) = when (keyExtInfo) {
            KEY_GET_SENSOR_LOCATION -> initExtString()
            else -> ""
        }

        override fun setExtInfo(authType: Int, keyExtInfo: String, valExtInfo: String) {
            // Do nothing
        }

        override fun getEnabled(bioType: Int): Int {
            if (!keyguardManager.isKeyguardSecure) {
                Log.e(LOG_TAG, "No secure keyguard set.")
                return BIOMETRIC_NOUSE_NOSET_KEYGUARD
            }

            return when (bioType) {
                AUTH_TYPE_FINGERPRINT -> when {
                    !fingerprintManager.isHardwareDetected -> {
                        Log.e(LOG_TAG, "Fingerprint hardware not available!")
                        BIOMETRIC_NOUSE_SYSTEMLOCKED
                    }

                    fingerprintManager.enrolledFingerprints.isNullOrEmpty() -> {
                        Log.e(LOG_TAG, "Fingerprint not enrolled!")
                        BIOMETRIC_NOUSE_NOT_ENROLLED
                    }

                    else -> BIOMETRIC_USE_READY
                }

                else -> 0
            }
        }

        override fun getIDList(bioType: Int): IntArray? = when (bioType) {
            AUTH_TYPE_FINGERPRINT -> {
                val enrolledFingerprintIds = fingerprintManager.enrolledFingerprints?.map {
                    it.biometricId
                }?.toIntArray()

                Log.w(LOG_TAG, "getIDList: ${enrolledFingerprintIds}!")
                enrolledFingerprintIds
            }

            else -> null
        }
    }

    private fun getFpPayService() = _fpPayService ?: run {
        val binder = ServiceManager.getService("${IFingerprintPay.DESCRIPTOR}/default")
        IFingerprintPay.Stub.asInterface(binder)?.also {
            binder.linkToDeath(fpPayServiceDeathRecipient, 0)
            _fpPayService = it
        }
    }

    private val defaultDisplayDimension by lazy {
        val dim1 = displayNoVerify?.supportedModes?.maxByOrNull { it.physicalHeight }
            ?.run { Point(physicalWidth, physicalHeight) }
        val dim2 = windowManager.maximumWindowMetrics.bounds
            .run { Point(width(), height()) }

        dim1?.let { Point(maxOf(dim1.x, dim2.x), maxOf(dim1.y, dim2.y)) } ?: dim2
    }

    private val iconDiameter by lazy { SystemProperties.getInt(FP_ICON_SIZE_PROP, 190) }

    // Coordinates of the upper left corner
    private val iconLocation by lazy {
        val iconLocationBottom = SystemProperties.getInt(FP_ICON_LOCATION_PROP, 278)
        Point(
            (defaultDisplayDimension.x - iconDiameter) / 2,
            defaultDisplayDimension.y - iconLocationBottom - iconDiameter / 2
        )
    }

    private fun initExtString() = run {
        val displayDimension = windowManager.maximumWindowMetrics.bounds.run {
            Point(width(), height())
        }
        val scale = { pos: Point ->
            Point(
                pos.x * displayDimension.x / defaultDisplayDimension.x,
                pos.y * displayDimension.y / defaultDisplayDimension.y
            )
        }
        val scaledLocation = scale(iconLocation)
        val scaledDiameter = scale(Point(iconDiameter, iconDiameter))

        JSONObject().apply {
            put("type", 0)
            put("fullView", JSONObject().apply {
                put("startX", scaledLocation.x)
                put("startY", scaledLocation.y)
                put("width", scaledDiameter.x)
                put("height", scaledDiameter.y)
                put("navConflict", true)
            })
        }.toString()
    }

    companion object {
        private val LOG_TAG = IfaaService::class.simpleName!!

        private const val AUTH_TYPE_NOT_SUPPORT = 0
        private const val AUTH_TYPE_FINGERPRINT = 1
        private const val AUTH_TYPE_OPTICAL_FINGERPRINT = 1.shl(4).or(AUTH_TYPE_FINGERPRINT)

        private const val BIOMETRIC_USE_READY = 1000
        private const val BIOMETRIC_NOUSE_SYSTEMLOCKED = 1001
        private const val BIOMETRIC_NOUSE_NOT_ENROLLED = 1002
        private const val BIOMETRIC_NOUSE_NOSET_KEYGUARD = 1003

        private const val COMMAND_OK = 0
        private const val COMMAND_FAIL = -1

        // Populated by fingerprint HAL
        private const val FP_TYPE_PROP = "persist.vendor.fingerprint.sensor_type"
        private const val FP_ICON_SIZE_PROP = "persist.vendor.fingerprint.optical.iconsize"
        private const val FP_ICON_LOCATION_PROP = "persist.vendor.fingerprint.optical.iconlocation"

        // NOTE: Populate ifaaModel from /my_stock/etc/sys_alipay_model_list.json
        private const val IFAA_MODEL_PROP = "sys.oplus.ifaa.model"

        private const val KEY_GET_SENSOR_LOCATION = "org.ifaa.ext.key.GET_SENSOR_LOCATION"
    }
}
