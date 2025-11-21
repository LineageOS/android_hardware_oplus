/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package com.oplus.evolution;

import android.net.Uri;
import android.os.Bundle;
import android.os.IInterface;

import com.android.internal.telephony.IIntegerConsumer;

import com.oplus.evolution.NetworklessDevice;
import com.oplus.internal.evolution.INetworklessCallback;
import com.oplus.internal.evolution.INetworklessConsumer;

import java.util.List;

public interface INetworkless extends IInterface {
    default int getCallState(String subId) {
        return 0;
    }

    default boolean getCommunictionForSub(String subId, int type) {
        return false;
    }

    default List<NetworklessDevice> getCurrentScanDevice(String subId) {
        return null;
    }

    default String getMsisdnHash(String subId, int a, int b, int c) {
        return null;
    }

    default String getNickName(String subId) {
        return null;
    }

    default int getOperatingMode(String subId) {
        return 0;
    }

    default boolean isNetworklessEnable(String subId) {
        return false;
    }

    default void placeNetworklessCall(String subId, String caller, String callee, String token,
            String extInfo, IIntegerConsumer consumer) {}

    default boolean registerNetworklessCallback(String subId, int[] eventList,
            INetworklessCallback callback) {
        return false;
    }

    default void sendRawMessage(String subId, long token, String dest, int msgType,
            Bundle bundle, byte[] payload, INetworklessConsumer consumer) {}

    default void sendRawMessageUri(String subId, long token, String dest, int msgType,
            Bundle bundle, Uri uri, INetworklessConsumer consumer) {}

    default void sendTextMessage(String subId, long token, String dest, int msgType, String text,
            INetworklessConsumer consumer) {}

    default void setAudioParameters(String subId, String params) {}

    default boolean setCommunictionForSub(String subId, int type, boolean enabled) {
        return false;
    }

    default boolean setNetworklessEnable(String subId, boolean enabled) {
        return false;
    }

    default boolean setNickName(String subId, String name) {
        return false;
    }

    default boolean setOperatingMode(String subId, int mode) {
        return false;
    }

    default boolean startScanDevices(String subId) {
        return false;
    }

    default void stimulateBlr(String subId) {}

    default boolean stopScanDevices(String subId) {
        return false;
    }

    default boolean unregisterNetworklessCallback(String subId, INetworklessCallback callback) {
        return false;
    }
}
