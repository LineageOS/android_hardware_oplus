package com.oplus.wrapper.view;

import android.graphics.Point;
import android.os.IBinder;
import android.os.IInterface;
import android.os.RemoteException;
import android.view.IRotationWatcher;

public interface IWindowManager {

    int getDockedStackSide() throws RemoteException;
    int getInitialDisplayDensity(int displayId) throws RemoteException;
    boolean hasNavigationBar(int displayId) throws RemoteException;
    int watchRotation(IRotationWatcher watcher, int displayId) throws RemoteException;
    void removeRotationWatcher(IRotationWatcher watcher) throws RemoteException;
    void setForcedDisplayDensityForUser(int displayId, int density, int userId) throws RemoteException;
    int getBaseDisplayDensity(int displayId) throws RemoteException;
    void getBaseDisplaySize(int displayId, Point size) throws RemoteException;

    public static abstract class Stub implements IInterface, android.view.IWindowManager {

        private final android.view.IWindowManager mIWindowManager;

        public Stub(android.view.IWindowManager windowManager) {
            mIWindowManager = windowManager;
        }

        public static IWindowManager asInterface(IBinder obj) {
            return new Proxy(android.view.IWindowManager.Stub.asInterface(obj));
        }

        @Override
        public IBinder asBinder() {
            return mIWindowManager.asBinder();
        }

        public static class Proxy implements IWindowManager {

            private final android.view.IWindowManager mIWindowManager;

            Proxy(android.view.IWindowManager windowManager) {
                mIWindowManager = windowManager;
            }

            @Override
            public int getDockedStackSide() throws RemoteException {
                return mIWindowManager.getDockedStackSide();
            }

            @Override
            public int getInitialDisplayDensity(int displayId) throws RemoteException {
                return mIWindowManager.getInitialDisplayDensity(displayId);
            }

            @Override
            public boolean hasNavigationBar(int displayId) throws RemoteException {
                return mIWindowManager.hasNavigationBar(displayId);
            }

            @Override
            public int watchRotation(IRotationWatcher watcher, int displayId) throws RemoteException {
                return mIWindowManager.watchRotation(watcher, displayId);
            }

            @Override
            public void removeRotationWatcher(IRotationWatcher watcher) throws RemoteException {
                mIWindowManager.removeRotationWatcher(watcher);
            }

            @Override
            public void setForcedDisplayDensityForUser(int displayId, int density, int userId) throws RemoteException {
                mIWindowManager.setForcedDisplayDensityForUser(displayId, density, userId);
            }

            @Override
            public int getBaseDisplayDensity(int displayId) throws RemoteException {
                return mIWindowManager.getBaseDisplayDensity(displayId);
            }

            @Override
            public void getBaseDisplaySize(int displayId, Point size) throws RemoteException {
                mIWindowManager.getBaseDisplaySize(displayId, size);
            }
        }
    }
}
