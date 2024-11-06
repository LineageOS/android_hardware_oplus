package com.oplus.wrapper.os;

public class SystemProperties {

    public static String get(String key) {
        return android.os.SystemProperties.get(key, "");
    }

    public static String get(String key, String def) {
        return android.os.SystemProperties.get(key, def);
    }

    public static int getInt(String key, int def) {
        return android.os.SystemProperties.getInt(key, def);
    }

    public static boolean getBoolean(String key, boolean def) {
        return android.os.SystemProperties.getBoolean(key, def);
    }

    public static void set(String key, String val) {
        android.os.SystemProperties.set(key, val);
    }
}
