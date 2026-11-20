/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.touchmonitor

import android.os.IBinder
import android.os.RemoteException
import android.os.ServiceManager
import android.util.Log
import vendor.oplus.hardware.touch.IOplusTouch

class TouchHal(private val onServiceDied: () -> Unit) {
    private var service: IOplusTouch? = null

    private val deathRecipient =
        IBinder.DeathRecipient {
            synchronized(this) { service = null }
            onServiceDied()
        }

    @Synchronized
    private fun getService(): IOplusTouch? {
        service?.let {
            return it
        }
        val binder = ServiceManager.waitForDeclaredService(SERVICE_NAME) ?: return null
        return try {
            binder.linkToDeath(deathRecipient, 0)
            IOplusTouch.Stub.asInterface(binder).also { service = it }
        } catch (e: RemoteException) {
            Log.e(TAG, "Failed to link to touch HAL", e)
            null
        }
    }

    fun writeNode(nodeFlag: Int, value: String) {
        val service = getService() ?: return
        for (deviceId in DEVICE_IDS) {
            try {
                if (service.isTouchNodeSupport(deviceId, nodeFlag) > 0) {
                    service.touchWriteNodeFile(deviceId, nodeFlag, value)
                }
            } catch (e: RemoteException) {
                Log.e(TAG, "Failed to write node $nodeFlag of device $deviceId", e)
            }
        }
    }

    companion object {
        private const val TAG = "OplusTouchMonitorHal"

        private val SERVICE_NAME = "${IOplusTouch.DESCRIPTOR}/default"
        private val DEVICE_IDS = intArrayOf(0, 1)
    }
}
