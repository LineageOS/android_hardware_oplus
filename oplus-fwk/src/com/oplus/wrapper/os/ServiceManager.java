package com.oplus.wrapper.os;

import android.os.IBinder;

public class ServiceManager {

    public static IBinder checkService(String name) {
        return android.os.ServiceManager.checkService(name);
    }

    public static IBinder getService(String name) {
        return android.os.ServiceManager.getService(name);
    }

    public static void addService(String name, IBinder token) {
        android.os.ServiceManager.addService(name, token);
    }
}
