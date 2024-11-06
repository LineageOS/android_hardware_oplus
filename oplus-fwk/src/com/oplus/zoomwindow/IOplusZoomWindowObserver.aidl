package com.oplus.zoomwindow;

import com.oplus.zoomwindow.OplusZoomWindowInfo;

interface IOplusZoomWindowObserver {

    void onInputMethodChanged(boolean isShown);
    void onZoomWindowDied(String appName);
    void onZoomWindowHide(inout OplusZoomWindowInfo info);
    void onZoomWindowShow(inout OplusZoomWindowInfo info);
}
