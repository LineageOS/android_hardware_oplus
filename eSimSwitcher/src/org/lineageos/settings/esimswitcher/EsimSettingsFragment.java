/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher;

import android.os.Bundle;
import android.os.Handler;

import androidx.preference.Preference;
import androidx.preference.PreferenceFragment;
import androidx.preference.SwitchPreferenceCompat;

import org.lineageos.settings.esimswitcher.R;

public class EsimSettingsFragment extends PreferenceFragment
        implements Preference.OnPreferenceChangeListener {

    private static final String ESIM_TOGGLE_KEY = "esim_toggle";

    private Handler mHandler = new Handler();

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        addPreferencesFromResource(R.xml.esim_settings);

        SwitchPreferenceCompat switcher = findPreference(ESIM_TOGGLE_KEY);
        switcher.setOnPreferenceChangeListener(this);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        mHandler.post(() -> EsimController.changeEsimState((Boolean) newValue ? 1 : 0));

        return true;
    }
}
