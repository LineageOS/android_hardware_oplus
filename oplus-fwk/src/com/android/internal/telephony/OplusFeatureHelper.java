/*
 * Copyright (C) 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package com.android.internal.telephony;

import com.oplus.content.OplusFeatureConfigManager;

import java.util.List;

public class OplusFeatureHelper {
    private static OplusFeatureHelper sInstance = null;

    public static OplusFeatureHelper getInstance() {
        if (sInstance == null) {
            sInstance = new OplusFeatureHelper();
        }
        return sInstance;
    }

    public boolean hasFeature(String featureName) {
        return false;
    }

    public boolean enableFeature(String featureName) {
        return true;
    }

    public boolean disableFeature(String featureName) {
        return true;
    }

    public void notifyFeaturesUpdate(String action, String actionValue) {}

    public boolean registerFeatureObserver(List<String> features, FeatureObserver observer) {
        return true;
    }

    public boolean unregisterFeatureObserver(FeatureObserver observer) {
        return true;
    }

    public static class FeatureObserver implements OplusFeatureConfigManager.OnFeatureObserver {}
}
