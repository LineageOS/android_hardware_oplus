package com.oplus.uifirst;

public class OplusUIFirstManager {
    
    private static OplusUIFirstManager sOplusUIFirstManager = null;

    public static OplusUIFirstManager getInstance() {
        if (sOplusUIFirstManager == null) {
            sOplusUIFirstManager = new OplusUIFirstManager();
        }
        return sOplusUIFirstManager;
    }

    public void setUxThreadValue(int pid, int tid, String value) {}
}
