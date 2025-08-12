/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package com.oplus.internal.telephony;

import android.os.RemoteException;
import android.os.SystemProperties;
import android.util.Log;

//import com.oplus.internal.telephony.regionlock.RegionLockConstants;

import vendor.oplus.hardware.esim.IOplusEsim;

public class OplusSimController {
    private static final String ESIM_GOOGLE_UPDATE_ACTION =
            "com.android.euicc.service.SIM_TYPE_UPDATE_ACTION";
    private static final String ESIM_STATUS_PROP = "persist.vendor.oplus.radio.esim.gpio.status";
    private static final String ESIM_SWITCH_PROP = "ro.vendor.oplus.esim.support";

    private static final String LOG_TAG = "OplusSimController";

    private IOplusEsim mOplusEsim;

    private int getEsimGpio() {
        int i = 1;
        try {
            if (mOplusEsim != null) {
                i = mOplusEsim.getEsimGpio();
            }
        } catch (RemoteException | RuntimeException e) {
            Log.e(LOG_TAG, "getEsimGpio: Exception: " + e);
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
            Log.e(LOG_TAG, "setEsimGpio: Exception: " + e);
            return 1;
        }
    }

    private int setUimPower(int i) {
        try {
            //if (mIsSupportSetSimType && i == 1) {
            //    return 0;
            //}
            if (mOplusEsim != null) {
                return mOplusEsim.setUimPower(i);
            }
            return 1;
        } catch (RemoteException | RuntimeException e) {
            Log.e(LOG_TAG, "setUimPower: Exception: " + e);
            return 1;
        }
    }

    private void changeEsimState(int i) {
        int gpio = getEsimGpio();
        Log.d(LOG_TAG, "current eSIM status = " + gpio);
        if (i == gpio) {
            Log.d(LOG_TAG, "no need to change eSIM state");
            return;
        }
        if (SystemProperties.get(ESIM_SWITCH_PROP)
                .equals("2" /* RegionLockConstants.UPDATE_AND_QUERY */)) {
            setUimPower(0);
            setEsimGpio(gpio == 0 ? 1 : 0);
            setUimPower(1);
        }
    }

}
