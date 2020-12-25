/*
 * Copyright (C) 2021 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.lineageos.euicc

import android.content.Context
import android.content.pm.PackageManager
import android.util.Log

class EuiccDisabler {
    companion object {
        private const val TAG = "OplusEuiccDisabler"

        private val EUICC_DEPENDENCIES = arrayOf(
            "com.google.android.gms",
            "com.google.android.gsf"
        )

        private val EUICC_PACKAGES = arrayOf(
            "com.google.android.euicc",
            "com.google.euiccpixel",
            "com.google.android.ims"
        )

        private fun isInstalledAndEnabled(pm: PackageManager, pkgName: String): Boolean {
            return try {
                val info = pm.getPackageInfo(pkgName, 0)
                Log.d(TAG, "package $pkgName installed, enabled = ${info.applicationInfo.enabled}")
                info.applicationInfo.enabled
            } catch (e: PackageManager.NameNotFoundException) {
                Log.d(TAG, "package $pkgName is not installed")
                false
            }
        }

        private fun shouldDisable(pm: PackageManager): Boolean {
            for (dep in EUICC_DEPENDENCIES) {
                if (!isInstalledAndEnabled(pm, dep)) {
                    // Disable if any of the dependencies are disabled
                    return true
                }
            }
            return false
        }

        fun enableOrDisableEuicc(context: Context) {
            val pm = context.packageManager
            val disable = shouldDisable(pm)
            val flag = if (disable) PackageManager.COMPONENT_ENABLED_STATE_DISABLED
                       else PackageManager.COMPONENT_ENABLED_STATE_ENABLED

            for (pkg in EUICC_PACKAGES) {
                pm.setApplicationEnabledSetting(pkg, flag, 0)
            }
        }
    }
}
