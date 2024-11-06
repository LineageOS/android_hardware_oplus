package com.oplus.app;

import android.os.UserHandle;

public class OPlusAccessControlManager {
    
    private static OPlusAccessControlManager sOPlusAccessControlManager = null;

    public static final int USER_CURRENT = UserHandle.myUserId();

    public static OPlusAccessControlManager getInstance() {
        if (sOPlusAccessControlManager == null) {
            sOPlusAccessControlManager = new OPlusAccessControlManager();
        }
        return sOPlusAccessControlManager;
    }

    public boolean isEncryptPass(String packageName, int userId) {
        return true;
    }

    public boolean isEncryptedPackage(String packageName, int userId) {
        return true;
    }
}
