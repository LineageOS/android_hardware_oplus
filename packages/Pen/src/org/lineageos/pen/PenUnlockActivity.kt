/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.pen

import android.app.Activity
import android.app.KeyguardManager
import android.content.Intent
import android.os.Bundle

class PenUnlockActivity : Activity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val pencilAddr = intent.getStringExtra(PenService.EXTRA_PENCIL_ADDR)
        if (pencilAddr == null) {
            finish()
            return
        }

        getSystemService(KeyguardManager::class.java)
            .requestDismissKeyguard(
                this,
                object : KeyguardManager.KeyguardDismissCallback() {
                    override fun onDismissSucceeded() {
                        startService(
                            Intent(this@PenUnlockActivity, PenService::class.java).apply {
                                putExtra(PenService.EXTRA_PENCIL_ADDR, pencilAddr)
                            }
                        )
                        finish()
                    }

                    override fun onDismissCancelled() {
                        finish()
                    }

                    override fun onDismissError() {
                        finish()
                    }
                },
            )
    }
}
