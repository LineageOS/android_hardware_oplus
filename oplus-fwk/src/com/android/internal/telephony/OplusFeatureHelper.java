package com.android.internal.telephony;

import android.util.Log;

import com.oplus.content.OplusFeatureConfigManager;

import java.util.List;

public class OplusFeatureHelper {

    private static final String TAG = "OplusFeatureHelper";

    private static OplusFeatureHelper sOplusFeatureHelper = null;

    public static OplusFeatureHelper getInstance() {
        if (sOplusFeatureHelper == null) {
            sOplusFeatureHelper = new OplusFeatureHelper();
        }
        return sOplusFeatureHelper;
    }

    public boolean hasFeature(String featureName) {
        Log.d(TAG, "hasFeature, feature=" + featureName);
        return false;
    }

    public boolean enableFeature(String featureName) {
        Log.d(TAG, "enableFeature, feature=" + featureName);
        return true;
    }

    public boolean disableFeature(String featureName) {
        Log.d(TAG, "disableFeature, feature=" + featureName);
        return true;
    }

    public void notifyFeaturesUpdate(String action, String actionValue) {
    }

    public boolean registerFeatureObserver(List<String> features, FeatureObserver observer) {
        return true;
    }

    public boolean unregisterFeatureObserver(FeatureObserver observer) {
        return true;
    }

    public static class FeatureObserver implements OplusFeatureConfigManager.OnFeatureObserver {
        @Override
        public void onFeatureUpdate(List<String> features) {
        }
    }
}
