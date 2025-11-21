/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package com.oplus.internal.evolution;

import android.os.Bundle;
import android.os.IInterface;

import com.oplus.evolution.NetworklessDevice;

import java.util.List;

public interface INetworklessCallback extends IInterface {
    default void onCallStateChanged(int state) {}
    default void onDeviceDiscoveryFinished(List<NetworklessDevice> devices) {}
    default void onDeviceSupportChanged(boolean supported) {}
    default void onNLSmsSupportChanged(boolean supported) {}
    default void onNearDevicesChanges(int event, List<NetworklessDevice> devices) {}
    default void onNetworklessInfo(Bundle bundle) {}
    default void onOperatingStateChanged(int state) {}
    default void onSwitchStateChanged(boolean enabled) {}
}
