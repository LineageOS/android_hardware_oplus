/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import android.os.IBinder
import android.os.RemoteException
import android.os.ServiceManager
import android.util.Log
import vendor.oplus.hardware.touch.IOplusTouch
import vendor.oplus.hardware.touch.IOplusTouchEventCallback
import vendor.oplus.hardware.touch.OplusTouchInfo
import vendor.oplus.hardware.touch.OplusTouchStatus

class TouchHal(private val onCommand: (String) -> Unit) {
    private var service: IOplusTouch? = null
    private var isCallbackRegistered = false

    private val callback =
        object : IOplusTouchEventCallback.Stub() {
            override fun touchSendCommand(clientFlag: Int, info: OplusTouchInfo) {
                // Synchronous variant is unused by the panel
            }

            override fun touchSendCommandOneWay(clientFlag: Int, info: OplusTouchInfo) {
                if (clientFlag != 0) {
                    Log.w(TAG, "Ignoring command for clientFlag=$clientFlag")
                    return
                }
                info.info?.let(onCommand)
            }

            override fun getInterfaceVersion() = IOplusTouchEventCallback.VERSION

            override fun getInterfaceHash(): String = IOplusTouchEventCallback.HASH
        }

    private val deathRecipient = IBinder.DeathRecipient { onServiceDied() }

    @Synchronized
    private fun getService(): IOplusTouch? {
        service?.let {
            return it
        }
        val binder = ServiceManager.waitForDeclaredService(SERVICE_NAME)
        if (binder == null) {
            Log.e(TAG, "$SERVICE_NAME is not declared")
            return null
        }
        return try {
            binder.linkToDeath(deathRecipient, 0)
            IOplusTouch.Stub.asInterface(binder).also { service = it }
        } catch (e: RemoteException) {
            Log.e(TAG, "Failed to link to touch HAL", e)
            null
        }
    }

    @Synchronized
    private fun onServiceDied() {
        Log.w(TAG, "Touch HAL died")
        val wasRegistered = isCallbackRegistered
        service = null
        isCallbackRegistered = false
        if (wasRegistered) {
            registerCallback()
        }
    }

    @Synchronized
    fun registerCallback() {
        if (isCallbackRegistered) {
            return
        }
        try {
            val ret = getService()?.registerEventCallback(callback) ?: return
            isCallbackRegistered = ret == OplusTouchStatus.OK
            if (!isCallbackRegistered) {
                Log.e(TAG, "registerEventCallback returned $ret")
            }
        } catch (e: RemoteException) {
            Log.e(TAG, "Failed to register touch event callback", e)
        }
    }

    @Synchronized
    fun unregisterCallback() {
        if (!isCallbackRegistered) {
            return
        }
        isCallbackRegistered = false
        try {
            service?.unregisterEventCallback(callback)
        } catch (e: RemoteException) {
            Log.e(TAG, "Failed to unregister touch event callback", e)
        }
    }

    fun writeBtInfo(info: String) {
        try {
            getService()?.touchWriteBtInfo(0, PenProtocol.NODE_BT_INFO, info)
        } catch (e: RemoteException) {
            Log.e(TAG, "touchWriteBtInfo failed", e)
        }
    }

    fun writeNode(nodeFlag: Int, value: String) {
        try {
            getService()?.touchWriteNodeFile(0, nodeFlag, value)
        } catch (e: RemoteException) {
            Log.e(TAG, "touchWriteNodeFile($nodeFlag) failed", e)
        }
    }

    companion object {
        private const val TAG = "OplusPenTouchHal"

        private val SERVICE_NAME = "${IOplusTouch.DESCRIPTOR}/default"
    }
}
