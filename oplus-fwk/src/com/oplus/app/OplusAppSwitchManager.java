package com.oplus.app;

import android.content.Context;

public class OplusAppSwitchManager {

    private static OplusAppSwitchManager sOplusAppSwitchManager = null;

    public static int APP_SWITCH_VERSION = 1;

    public static OplusAppSwitchManager getInstance() {
        if (sOplusAppSwitchManager == null) {
            sOplusAppSwitchManager = new OplusAppSwitchManager();
        }
        return sOplusAppSwitchManager;
    }

    public boolean registerAppSwitchObserver(Context context,
            OnAppSwitchObserver observer, OplusAppSwitchConfig config) {
        return true;
    }

    public boolean unregisterAppSwitchObserver(Context context, OnAppSwitchObserver observer) {
        return true;
    }

    public interface OnAppSwitchObserver {

        void onActivityEnter(OplusAppEnterInfo oplusAppEnterInfo);

        void onActivityExit(OplusAppExitInfo oplusAppExitInfo);

        void onAppEnter(OplusAppEnterInfo oplusAppEnterInfo);

        void onAppExit(OplusAppExitInfo oplusAppExitInfo);
    }
}
