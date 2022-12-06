/*
 * Copyright (C) 2021-2022 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.device

import android.os.Bundle

import com.android.settingslib.collapsingtoolbar.CollapsingToolbarBaseActivity
import com.android.settingslib.widget.R

class ButtonSettingsActivity : CollapsingToolbarBaseActivity() {
    public override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        fragmentManager.beginTransaction().replace(
            R.id.content_frame,
            ButtonSettingsFragment()
        ).commit()
    }
}
