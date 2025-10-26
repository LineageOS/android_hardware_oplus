/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.settings.esimswitcher;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.SystemProperties;
import android.telephony.SubscriptionInfo;
import android.telephony.SubscriptionManager;
import android.provider.Settings;
import android.util.Log;

//import com.oplus.internal.telephony.regionlock.RegionLockConstants;

import vendor.oplus.hardware.esim.IOplusEsim;

import java.util.Iterator;
import java.util.List;

public class EsimController extends Service {
    private static final boolean DEBUG = true;

    private static final String ESIM_GOOGLE_UPDATE_ACTION =
            "com.android.euicc.service.SIM_TYPE_UPDATE_ACTION";
    private static final String ESIM_STATUS_PROP = "persist.vendor.oplus.radio.esim.gpio.status";
    private static final String ESIM_SWITCH_PROP = "ro.vendor.oplus.esim.support";
    private static final String LOG_TAG = "OplusSimController";
    private static final String READ_PRIVILEGED_PHONE_STATE =
            "android.permission.READ_PRIVILEGED_PHONE_STATE";
    private static final String SIM_TYPE_EXTRA_SLOT_KEY =
            "com.android.euicc.service.extra_slot_id";
    private static final String SIM_TYPE_EXTRA_TYPE_KEY =
            "com.android.euicc.service.extra_sim_type";

    private Context mContext;

    private static IOplusEsim mOplusEsim;

    @Override
    public void onCreate() {
        if (DEBUG) Log.d(LOG_TAG, "Creating service");

        final String fqName = IOplusEsim.DESCRIPTOR + "/default";

        try {
            mOplusEsim = IOplusEsim.Stub.asInterface(ServiceManager.getService(fqName));
        } catch (Exception e) {
            Log.e(LOG_TAG, "Failed to get OplusEsim service: " + e);
        }
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (DEBUG) Log.d(LOG_TAG, "Starting service");
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        if (DEBUG) Log.d(LOG_TAG, "Destroying service");
        super.onDestroy();
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    private BroadcastReceiver mEuiccGoogleReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            List availableSubscriptionInfoList =
                    SubscriptionManager.from(mContext).getAvailableSubscriptionInfoList();
            if (intent.getAction().equals(ESIM_GOOGLE_UPDATE_ACTION)) {
                int slot = intent.getIntExtra(SIM_TYPE_EXTRA_SLOT_KEY, -1);
                int type = intent.getIntExtra(SIM_TYPE_EXTRA_TYPE_KEY, -1);
                boolean shouldChangeEsimState = true;
                Log.d(LOG_TAG, "open eSIM logicalSlotId = " + slot + ", mode = " + type);
                if (hasSetupCompleted() && getEsimGpio() == 1 && type == 0
                        && availableSubscriptionInfoList != null
                        && availableSubscriptionInfoList.size() > 0) {
                    Iterator it = availableSubscriptionInfoList.iterator();
                    while (it.hasNext()) {
                        if (((SubscriptionInfo) it.next()).isEmbedded()) {
                            shouldChangeEsimState = false;
                        }
                    }
                }
                if (shouldChangeEsimState) {
                    changeEsimState(type);
                    return;
                }
                Log.d(LOG_TAG, "changing eSIM state disallowed because core has profile");
            }
        }
    };

    private BroadcastReceiver mPowerOffReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(Intent.ACTION_SHUTDOWN)) {
                setUimPower(0);
            }
        }
    };

    private boolean hasSetupCompleted() {
        return Settings.Secure.getInt(mContext.getContentResolver(),
                Settings.Secure.USER_SETUP_COMPLETE, 0) != 0;
    }

    private static int getEsimGpio() {
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

    private static int setEsimGpio(int i) {
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

    private static int setUimPower(int i) {
        try {
            //if (mIsSupportSetSimType && i == 1) {
            //    return 0;
            //}
            if (mOplusEsim != null) {
                return mOplusEsim.setUimPower(i);
            }
            return 1;
        } catch (RemoteException | RuntimeException e) {
            Log.e(LOG_TAG, "setUimPower", e);
            return 1;
        }
    }

    public static void changeEsimState(int i) {
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
}
