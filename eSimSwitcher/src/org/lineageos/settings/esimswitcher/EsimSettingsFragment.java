/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.text.TextUtils;

import androidx.preference.Preference;
import androidx.preference.PreferenceFragment;
import androidx.preference.PreferenceManager;
import androidx.preference.SwitchPreferenceCompat;

import org.lineageos.settings.esimswitcher.R;

public class EsimSettingsFragment extends PreferenceFragment
        implements Preference.OnPreferenceChangeListener {

    private Handler mHandler = new Handler();

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        addPreferencesFromResource(R.xml.esim_settings);
    }

    @Override
    public void onResume() {
        super.onResume();
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        mHandler.post(() -> EsimService.changeEsimState(Integer.parseInt(newValue.toString())));

        return true;
    }
}
