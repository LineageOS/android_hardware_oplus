/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package com.oplus.evolution;

import android.net.Uri;
import android.content.Context;
import android.os.Bundle;

import com.android.internal.telephony.IIntegerConsumer;

import com.oplus.evolution.NetworklessDevice;
import com.oplus.internal.evolution.INetworklessCallback;
import com.oplus.internal.evolution.INetworklessConsumer;

import java.util.List;

public class NetworklessManager {
    private static NetworklessManager sInstance = null;

    public NetworklessManager(Context context) {}

    public static NetworklessManager getInstance(Context context) {
        if (sInstance == null) {
            sInstance = new NetworklessManager(context);
        }
        return sInstance;
    }

    public int getCallState(String subId) {
        return 0;
    }

    public boolean getCommunictionForSub(String subId, int type) {
        return false;
    }

    public List<NetworklessDevice> getCurrentScanDevice(String subId) {
        return null;
    }

    public String getMsisdnHash(String subId, int a, int b, int c) {
        return null;
    }

    public String getNickName(String subId) {
        return null;
    }

    public int getOperatingMode(String subId) {
        return 0;
    }

    public boolean isNetworklessEnable(String subId) {
        return false;
    }

    public void placeNetworklessCall(String subId, String caller, String callee, String token,
            String extInfo, IIntegerConsumer consumer) {}

    public boolean registerNetworklessCallback(String subId, int[] eventList,
            INetworklessCallback callback) {
        return false;
    }

    public void sendRawMessage(String subId, long token, String dest, int msgType,
            Bundle bundle, byte[] payload, INetworklessConsumer consumer) {}

    public void sendRawMessageUri(String subId, long token, String dest, int msgType,
            Bundle bundle, Uri uri, INetworklessConsumer consumer) {}

    public void sendTextMessage(String subId, long token, String dest, int msgType, String text,
            INetworklessConsumer consumer) {}

    public void setAudioParameters(String subId, String params) {}

    public boolean setCommunictionForSub(String subId, int type, boolean enabled) {
        return false;
    }

    public boolean setNetworklessEnable(String subId, boolean enabled) {
        return false;
    }

    public boolean setNickName(String subId, String name) {
        return false;
    }

    public boolean setOperatingMode(String subId, int mode) {
        return false;
    }

    public boolean startScanDevices(String subId) {
        return false;
    }

    public void stimulateBlr(String subId) {}

    public boolean stopScanDevices(String subId) {
        return false;
    }

    public boolean unregisterNetworklessCallback(String subId, INetworklessCallback callback) {
        return false;
    }
}
