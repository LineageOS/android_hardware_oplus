/*
 * Copyright (C) 2021 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.doze

import android.os.Bundle
import android.preference.PreferenceActivity

class DozeSettingsActivity : PreferenceActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        fragmentManager
            .beginTransaction()
            .replace(android.R.id.content, DozeSettingsFragment(), TAG)
            .commit()
    }

    companion object {
        private const val TAG = "DozeSettingsActivity"
    }
}
