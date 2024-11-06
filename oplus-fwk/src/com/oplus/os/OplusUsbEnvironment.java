package com.oplus.os;

import android.content.Context;
import android.os.Environment;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.UserHandle;
import android.os.storage.DiskInfo;
import android.os.storage.IStorageManager;
import android.os.storage.StorageManager;
import android.os.storage.VolumeInfo;
import android.util.Log;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class OplusUsbEnvironment extends Environment {

    private static final String TAG = "OplusUsbEnvironment";
    private static final String DEFAULT_INTERNAL_PATH = "/storage/emulated/0";

    private static IStorageManager sMountService = null;
    private static Object sLock = new Object();
    private static String sExternalSdDir = null;
    private static String sInternalSdDir = DEFAULT_INTERNAL_PATH;
    private static ArrayList<String> sOtgPathes = new ArrayList<>();

    private static void update(Context context) {
        if (sMountService == null) {
            sMountService = IStorageManager.Stub.asInterface(ServiceManager.getService("mount"));
        }
        getVolumes();
    }

    public static File getExternalSdDirectory(Context context) {
        String path;
        synchronized (sLock) {
            update(context);
            path = sExternalSdDir;
        }
        if (path == null) {
            return null;
        }
        return new File(path);
    }

    public static String getExternalSdState(Context context) {
        String path;
        final StorageManager sm = context.getSystemService(StorageManager.class);
        synchronized (sLock) {
            update(context);
            path = sExternalSdDir;
        }
        if (path == null || sm == null) {
            return "unknown";
        }
        return sm.getVolumeState(path);
    }


    public static String getExternalPath(Context context) {
        String path;
        synchronized (sLock) {
            update(context);
            path = sExternalSdDir;
        }
        return path;
    }

    public static File getInternalSdDirectory(Context context) {
        String path;
        synchronized (sLock) {
            update(context);
            path = sInternalSdDir;
        }
        if (path == null) {
            return null;
        }
        return new File(path);
    }

    public static String getInternalSdState(Context context) {
        String path;
        final StorageManager sm = context.getSystemService(StorageManager.class);
        synchronized (sLock) {
            update(context);
            path = sInternalSdDir;
        }
        if (path == null || sm == null) {
            return "unknown";
        }
        return sm.getVolumeState(path);
    }


    public static String getInternalPath(Context context) {
        String path;
        synchronized (sLock) {
            update(context);
            path = sInternalSdDir;
        }
        return path;
    }

    public static List<String> getOtgPath(Context context) {
        synchronized (sLock) {
            update(context);
            if (sOtgPathes == null) {
                return null;
            }
            return (ArrayList<String>) sOtgPathes.clone();
        }
    }

    public static void getVolumes() {
        if (sMountService == null) {
            Log.e(TAG, "getVolumes: sMountService is null!!!");
            return;
        }
        try {
            final VolumeInfo[] vols = sMountService.getVolumes(0);
            sExternalSdDir = null;
            sOtgPathes.clear();
            for (VolumeInfo vol : vols) {
                final String path = vol.path;
                if (vol.type == 2) {
                    final int userId = UserHandle.myUserId();
                    if (path != null) {
                        sInternalSdDir = path.concat("/").concat(Integer.toString(userId));
                    }
                } else {
                    final DiskInfo diskInfo = vol.getDisk();
                    if (diskInfo != null) {
                        if (diskInfo.isSd() && path != null) {
                            sExternalSdDir = path;
                        }
                        if (diskInfo.isUsb() && path != null && !sOtgPathes.contains(path)) {
                            sOtgPathes.add(path);
                        }
                    }
                }
            }
        } catch (RemoteException e) {
        }
    }

    public static boolean isVolumeMounted(Context context, String path) {
        synchronized (sLock) {
            update(context);
        }
        final StorageManager sm = context.getSystemService(StorageManager.class);
        if (path == null || sm == null) {
            return false;
        }
        return "mounted".equals(sm.getVolumeState(path));
    }
}
