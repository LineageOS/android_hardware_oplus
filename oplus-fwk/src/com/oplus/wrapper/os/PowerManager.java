package com.oplus.wrapper.os;

public class PowerManager {

    private final android.os.PowerManager mPowerManager;

    public PowerManager(android.os.PowerManager powerManager) {
        mPowerManager = powerManager;
    }

    public int getMaximumScreenBrightnessSetting() {
        return mPowerManager.getMaximumScreenBrightnessSetting();
    }
}
