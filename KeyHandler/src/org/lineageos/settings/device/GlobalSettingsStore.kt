/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device

import android.content.ContentResolver
import android.provider.Settings
import androidx.preference.PreferenceDataStore

class GlobalSettingsStore(private val resolver: ContentResolver) : PreferenceDataStore() {

    override fun getBoolean(key: String, defValue: Boolean): Boolean {
        return Settings.Global.getInt(resolver, key, if (defValue) 1 else 0) == 1
    }
    override fun putBoolean(key: String, value: Boolean) {
        Settings.Global.putInt(resolver, key, if (value) 1 else 0)
    }

    override fun getString(key: String, defValue: String?): String? {
        return Settings.Global.getString(resolver, key) ?: defValue
    }

    override fun putString(key: String, value: String?) {
        Settings.Global.putString(resolver, key, value)
    }
}
