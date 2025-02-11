/*
 * Copyright (C) 2021-2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "AreaCapture.h"

#include <android-base/properties.h>
#include <gui/AidlUtil.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/SyncScreenCaptureListener.h>
#include <ui/DisplayState.h>
#include <ui/PixelFormat.h>

using android::DisplayCaptureArgs;
using android::GraphicBuffer;
using android::IBinder;
using android::Rect;
using android::ScreenshotClient;
using android::sp;
using android::SurfaceComposerClient;
using android::SyncScreenCaptureListener;
using android::base::GetProperty;
using android::gui::ScreenCaptureResults;
using android::gui::aidl_utils::toARect;
using android::ui::PixelFormat;

namespace aidl {
namespace vendor {
namespace lineage {
namespace oplus_als {

AreaCapture::AreaCapture() {
    int left, top, right, bottom;
    std::istringstream is(GetProperty("vendor.sensors.als_correction.grabrect", ""));

    is >> left >> top >> right >> bottom;
    if (left == 0) {
        LOG_ALWAYS_FATAL("No screenshot grab area config");
        return;
    }

    ALOGI("Screenshot grab area: %d %d %d %d", left, top, right, bottom);
    m_screenshot_rect = Rect(left, top, right, bottom);
}

// See frameworks/base/services/core/jni/com_android_server_display_DisplayControl.cpp and
// frameworks/base/core/java/android/view/SurfaceControl.java
sp<IBinder> AreaCapture::getInternalDisplayToken() {
    const auto displayIds = SurfaceComposerClient::getPhysicalDisplayIds();
    return SurfaceComposerClient::getPhysicalDisplayToken(displayIds[0]);
}

ndk::ScopedAStatus AreaCapture::getAreaBrightness(AreaRgbCaptureResult* _aidl_return) {
    DisplayCaptureArgs displayCaptureArgs;
    displayCaptureArgs.displayToken = getInternalDisplayToken();
    displayCaptureArgs.captureArgs.pixelFormat = ::android::PIXEL_FORMAT_RGBA_8888;
    displayCaptureArgs.captureArgs.sourceCrop = toARect(m_screenshot_rect);
    displayCaptureArgs.width = m_screenshot_rect.getWidth();
    displayCaptureArgs.height = m_screenshot_rect.getHeight();
    displayCaptureArgs.captureArgs.captureSecureLayers = true;

    sp<SyncScreenCaptureListener> captureListener = new SyncScreenCaptureListener();
    if (ScreenshotClient::captureDisplay(displayCaptureArgs, captureListener) !=
        ::android::NO_ERROR) {
        ALOGE("Capture failed");
        return ndk::ScopedAStatus::fromServiceSpecificError(-1);
    }

    auto captureResults = captureListener->waitForResults();
    if (!captureResults.fenceResult.ok()) {
        ALOGE("Fence result error");
        return ndk::ScopedAStatus::fromServiceSpecificError(-1);
    }

    uint8_t* out;
    captureResults.buffer->lock(GraphicBuffer::USAGE_SW_READ_OFTEN, reinterpret_cast<void**>(&out));

    auto resultWidth = captureResults.buffer->getWidth();
    auto resultHeight = captureResults.buffer->getHeight();
    auto stride = captureResults.buffer->getStride();

    // we can sum this directly on linear light
    uint32_t rsum = 0, gsum = 0, bsum = 0;
    for (int y = 0; y < resultHeight; y++) {
        for (int x = 0; x < resultWidth; x++) {
            rsum += out[y * (stride * 4) + x * 4];
            gsum += out[y * (stride * 4) + x * 4 + 1];
            bsum += out[y * (stride * 4) + x * 4 + 2];
        }
    }

    float max = resultWidth * resultHeight;
    _aidl_return->r = rsum / max;
    _aidl_return->g = gsum / max;
    _aidl_return->b = bsum / max;

    captureResults.buffer->unlock();

    return ndk::ScopedAStatus::ok();
}

}  // namespace oplus_als
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
