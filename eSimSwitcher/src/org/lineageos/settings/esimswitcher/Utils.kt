/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher

import android.util.Log

object Utils {
    private const val TAG = "Utils"

    fun getSystemProperty(property: String, defaultValue: String = ""): String {
        return try {
            val clazz = Class.forName("android.os.SystemProperties")
            val getter = clazz.getDeclaredMethod("get", String::class.java)
            val value = getter.invoke(null, property) as String
            if (value.isNotEmpty()) value else defaultValue
        } catch (e: Exception) {
            Log.d(TAG, "Unable to read system properties")
            defaultValue
        }
    }
}
