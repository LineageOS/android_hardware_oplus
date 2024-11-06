package com.oplus.zoomwindow;

import android.os.Parcel;
import android.os.Parcelable;

public class OplusZoomWindowInfo implements Parcelable {

    public static final Parcelable.Creator<OplusZoomWindowInfo> CREATOR =
            new Parcelable.Creator<OplusZoomWindowInfo>() {

        @Override
        public OplusZoomWindowInfo createFromParcel(Parcel source) {
            return new OplusZoomWindowInfo(source);
        }

        @Override
        public OplusZoomWindowInfo[] newArray(int size) {
            return new OplusZoomWindowInfo[size];
        }
    };

    public OplusZoomWindowInfo() {}

    public OplusZoomWindowInfo(Parcel in) {}

    public OplusZoomWindowInfo(OplusZoomWindowInfo in) {}

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {}

    public void readFromParcel(Parcel in) {}
}
