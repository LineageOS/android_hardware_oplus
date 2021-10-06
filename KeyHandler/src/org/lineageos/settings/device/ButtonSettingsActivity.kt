/*
 * Copyright (C) 2021 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device

import android.R
import android.preference.PreferenceActivity
import android.os.Bundle

class ButtonSettingsActivity : PreferenceActivity() {
    public override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        fragmentManager.beginTransaction().replace(
            R.id.content,
            ButtonSettingsFragment()
        ).commit()
    }
}
