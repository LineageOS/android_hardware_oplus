package com.oplus.util;

import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;

public class OplusNetworkUtil {

    private static final String DEFAULT_HTTP_URI = "http://connectivitycheck.gstatic.com/generate_204";

    public static boolean isWifiConnected(Context context) {
        final ConnectivityManager cm = context.getSystemService(ConnectivityManager.class);
        if (cm != null) {
            return cm.getNetworkInfo(ConnectivityManager.TYPE_WIFI).getState()
                    == NetworkInfo.State.CONNECTED;
        }
        return false;
    }

    public static boolean isMobileDataConnected(Context context) {
        final ConnectivityManager cm = context.getSystemService(ConnectivityManager.class);
        if (cm != null) {
            return cm.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).getState()
                    == NetworkInfo.State.CONNECTED;
        }
        return false;
    }

    public static void onClickLoginBtn(Context context) {
        final Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(DEFAULT_HTTP_URI));
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_BROUGHT_TO_FRONT);
        context.startActivity(intent);
    }

    public static int getErrorString(Context context, String url) {
        return 1;
    }
}
