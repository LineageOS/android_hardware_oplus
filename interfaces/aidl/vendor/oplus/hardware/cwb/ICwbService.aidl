/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.cwb;

import android.os.ParcelFileDescriptor;
import vendor.oplus.hardware.cwb.Error;
import vendor.oplus.hardware.cwb.Histogram;
import vendor.oplus.hardware.cwb.ICwbCallback;
import vendor.oplus.hardware.cwb.Rect;
import vendor.oplus.hardware.cwb.Rgb;

@VintfStability
interface ICwbService {
    Error getRGBValue(in Rect rect, out Rgb rgb);
    Error getLumasValue(in Rect rect, inout float[] lumas);
    Error getHistogramValue(in Rect rect, out Histogram histogram);

    boolean getCwbPostProcessStatus();
    Error setCwbPostProcessStatus(boolean enabled);

    Error registerCallback(in ICwbCallback callback);
    Error unregisterCallback(in ICwbCallback callback);

    boolean enable();
    boolean disable();

    Error setDebug(in int[] debugArgs);
    Error getCwbValue(int mode, in Rect rect, inout int[] values);
    Error setCwbValue(int mode, in Rect rect, inout int[] values);

    Error initializeSharedMemory(int width, int height, in ParcelFileDescriptor sharedMemory);
    Error getSharedData(int mode, in Rect rect, in ParcelFileDescriptor sharedMemory, int size, inout int[] values);
    Error getScreenResolution(int mode, out int[] values);
    Error releaseSharedMemory(int width, int height);
}
