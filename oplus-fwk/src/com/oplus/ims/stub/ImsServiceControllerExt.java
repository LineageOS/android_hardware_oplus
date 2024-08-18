package com.oplus.ims.stub;

import android.content.Context;

import com.oplus.ims.IImsExt;

import java.util.concurrent.Executor;

public class ImsServiceControllerExt {

    private final ImsExtStub mImsExtStub;

    public ImsServiceControllerExt(Context context, Executor executor) {
        mImsExtStub = new ImsExtStub();
    }

    public IImsExt getIImsExt() {
        return mImsExtStub;
    }

    public static class ImsExtStub extends IImsExt.Stub {
    }
}
