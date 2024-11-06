package android.app;

import android.os.RemoteException;

import com.oplus.app.OplusAppInfo;

import java.util.ArrayList;
import java.util.List;

public class OplusActivityManager {

    private static OplusActivityManager sOplusActivityManager = null;
    private static ArrayList<OplusAppInfo> sTopAppInfos = new ArrayList<OplusAppInfo>();

    public static OplusActivityManager getInstance() {
        if (sOplusActivityManager == null) {
            sOplusActivityManager = new OplusActivityManager();
        }
        return sOplusActivityManager;
    }

    public List<OplusAppInfo> getAllTopAppInfos() throws RemoteException {
        return (ArrayList<OplusAppInfo>) sTopAppInfos.clone();
    }
}
