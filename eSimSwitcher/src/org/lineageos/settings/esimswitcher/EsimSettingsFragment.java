/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher;

import android.os.Bundle;
import android.os.Handler;
import android.os.RemoteException;
import android.os.ServiceManager;
//import android.os.SystemProperties;
import android.util.Log;

import androidx.preference.Preference;
import androidx.preference.PreferenceFragment;
import androidx.preference.SwitchPreferenceCompat;

import org.lineageos.settings.esimswitcher.R;

import vendor.oplus.hardware.esim.IOplusEsim;

public class EsimSettingsFragment extends PreferenceFragment
        implements Preference.OnPreferenceChangeListener {

    //private static final String ESIM_SWITCH_PROP = "ro.vendor.oplus.esim.support";
    private static final String ESIM_TOGGLE_KEY = "esim_toggle";
    private static final String LOG_TAG = "OplusEsimSwitcher";

    private Handler mHandler = new Handler();

    private IOplusEsim mOplusEsim;

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        addPreferencesFromResource(R.xml.esim_settings);

        mOplusEsim = getOplusEsim();

        SwitchPreferenceCompat switcher = findPreference(ESIM_TOGGLE_KEY);
        switcher.setOnPreferenceChangeListener(this);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        mHandler.post(() -> changeEsimState((Boolean) newValue ? 1 : 0));

        return true;
    }

    private synchronized IOplusEsim getOplusEsim() {
        final String fqName = IOplusEsim.DESCRIPTOR + "/default";

        try {
            return IOplusEsim.Stub.asInterface(ServiceManager.getService(fqName));
        } catch (Exception e) {
            Log.e(LOG_TAG, "Failed to get OplusEsim service: " + e);
            return null;
        }
    }

    private void changeEsimState(int i) {
        int gpio = getEsimGpio();
        Log.d(LOG_TAG, "current eSIM status = " + gpio);
        if (i == gpio) {
            Log.d(LOG_TAG, "no need to change eSIM state");
            return;
        }
        //if (SystemProperties.get(ESIM_SWITCH_PROP)
        //        .equals("2" /* RegionLockConstants.UPDATE_AND_QUERY */)) {
            setUimPower(0);
            setEsimGpio(gpio == 0 ? 1 : 0);
            setUimPower(1);
        //}
    }

    private int getEsimGpio() {
        int i = 1;
        try {
            if (mOplusEsim != null) {
                i = mOplusEsim.getEsimGpio();
            }
        } catch (RemoteException | RuntimeException e) {
            Log.e(LOG_TAG, "getEsimGpio", e);
        }
        return i;
    }

    private int setEsimGpio(int i) {
        try {
            if (mOplusEsim != null) {
                return mOplusEsim.setEsimGpio(i);
            }
            return 1;
        } catch (Exception e) {
            Log.e(LOG_TAG, "setEsimGpio", e);
            return 1;
        }
    }

    private int setUimPower(int i) {
        try {
            if (mOplusEsim != null) {
                return mOplusEsim.setUimPower(i);
            }
            return 1;
        } catch (RemoteException | RuntimeException e) {
            Log.e(LOG_TAG, "setUimPower", e);
            return 1;
        }
    }
}
