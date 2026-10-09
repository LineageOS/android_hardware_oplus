/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.le.BluetoothLeScanner
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanFilter
import android.bluetooth.le.ScanResult
import android.bluetooth.le.ScanSettings
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.Handler
import android.util.Log
import java.util.concurrent.Executors

class PenPairing(
    private val context: Context,
    private val handler: Handler,
    private val onFinished: (BluetoothDevice, Boolean) -> Unit,
) {
    private val bluetoothManager by lazy { context.getSystemService(BluetoothManager::class.java) }

    // Waiting for the LE scanner blocks when Bluetooth is being turned on
    private val executor = Executors.newSingleThreadExecutor()

    private var address: String? = null
    private var scan: Pair<BluetoothLeScanner, ScanCallback>? = null

    val isPairing
        get() = address != null

    private val timeoutRunnable = Runnable {
        address?.let { finish(bluetoothManager.adapter.getRemoteDevice(it), false) }
    }

    private val receiver =
        object : BroadcastReceiver() {
            override fun onReceive(context: Context, intent: Intent) {
                val device =
                    intent.getParcelableExtra(
                        BluetoothDevice.EXTRA_DEVICE,
                        BluetoothDevice::class.java,
                    ) ?: return
                if (!device.address.equals(address, ignoreCase = true)) {
                    return
                }
                val state =
                    intent.getIntExtra(BluetoothDevice.EXTRA_BOND_STATE, BluetoothDevice.ERROR)
                val previousState =
                    intent.getIntExtra(
                        BluetoothDevice.EXTRA_PREVIOUS_BOND_STATE,
                        BluetoothDevice.ERROR,
                    )
                when {
                    state == BluetoothDevice.BOND_BONDED -> finish(device, true)
                    state == BluetoothDevice.BOND_NONE &&
                        previousState == BluetoothDevice.BOND_BONDING -> finish(device, false)
                }
            }
        }

    fun start(pencilAddr: String) {
        if (address != null) {
            return
        }
        address = pencilAddr
        context.registerReceiver(
            receiver,
            IntentFilter(BluetoothDevice.ACTION_BOND_STATE_CHANGED),
            null,
            handler,
            Context.RECEIVER_EXPORTED,
        )
        handler.postDelayed(timeoutRunnable, PAIRING_TIMEOUT_MS)
        executor.execute { scanAndBond(pencilAddr) }
    }

    fun stop() {
        address?.let { cancel() }
        executor.shutdownNow()
    }

    private fun scanAndBond(pencilAddr: String) {
        val adapter = bluetoothManager.adapter
        @Suppress("DEPRECATION") adapter.enable()

        val scanner = run {
            repeat(SCANNER_WAIT_ATTEMPTS) {
                adapter.bluetoothLeScanner?.let {
                    return@run it
                }
                Thread.sleep(SCANNER_WAIT_INTERVAL_MS)
            }
            return@run null
        }
        if (scanner == null) {
            Log.e(TAG, "LE scanner unavailable")
            handler.post { finish(adapter.getRemoteDevice(pencilAddr), false) }
            return
        }

        val callback =
            object : ScanCallback() {
                override fun onScanResult(callbackType: Int, result: ScanResult) {
                    super.onScanResult(callbackType, result)
                    scanner.stopScan(this)

                    if (!result.device.createBond()) {
                        handler.post { finish(result.device, false) }
                    }
                }

                override fun onScanFailed(errorCode: Int) {
                    super.onScanFailed(errorCode)
                    Log.e(TAG, "Scan failed: $errorCode")
                    handler.post { finish(adapter.getRemoteDevice(pencilAddr), false) }
                }
            }
        handler.post {
            // Pairing may have finished or timed out while waiting for the scanner
            if (address == pencilAddr) {
                scan = scanner to callback
                scanner.startScan(
                    listOf(ScanFilter.Builder().setDeviceAddress(pencilAddr).build()),
                    ScanSettings.Builder()
                        .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY)
                        .setReportDelay(0L)
                        .build(),
                    callback,
                )
            }
        }
    }

    private fun finish(device: BluetoothDevice, isBonded: Boolean) {
        if (!device.address.equals(address, ignoreCase = true)) {
            return
        }
        cancel()
        onFinished(device, isBonded)
    }

    private fun cancel() {
        handler.removeCallbacks(timeoutRunnable)
        context.unregisterReceiver(receiver)
        scan?.let { (scanner, callback) -> scanner.stopScan(callback) }
        scan = null
        address = null
    }

    companion object {
        private const val TAG = "OplusPenPairing"

        private const val PAIRING_TIMEOUT_MS = 30_000L
        private const val SCANNER_WAIT_ATTEMPTS = 50
        private const val SCANNER_WAIT_INTERVAL_MS = 100L
    }
}
