package net.oneplus.odm;

import android.content.Context;

import java.util.Map;

public class OpDeviceManagerInjector {

    private static OpDeviceManagerInjector sOpDeviceManagerInjector = null;

    public void preserveAppData(Context a, String b, Map c, Map d) {}

    public static OpDeviceManagerInjector getInstance() {
        if (sOpDeviceManagerInjector == null) {
            sOpDeviceManagerInjector = new OpDeviceManagerInjector();
        }
        return sOpDeviceManagerInjector;
    }
}
