package android.app;

import android.content.Context;

import java.util.ArrayList;

public class OplusWhiteListManager {

    public OplusWhiteListManager(Context context) {}

    public ArrayList<String> getStageProtectListFromPkg(String calledPkg, int type) {
        return new ArrayList<>();
    }

    public void addStageProtectInfo(String pkg, long timeout) {}

    public void removeStageProtectInfo(String pkg) {}
}
