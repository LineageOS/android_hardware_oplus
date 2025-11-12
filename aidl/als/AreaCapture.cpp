/*
 * SPDX-FileCopyrightText: 2021-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "AreaCapture.h"

#include <android-base/logging.h>
#include <gui/AidlUtil.h>
#include <gui/SyncScreenCaptureListener.h>

using android::DisplayCaptureArgs;
using android::GraphicBuffer;
using android::IBinder;
using android::Rect;
using android::ScreenshotClient;
using android::sp;
using android::SurfaceComposerClient;
using android::SyncScreenCaptureListener;
using android::gui::aidl_utils::toARect;

namespace aidl {
namespace vendor {
namespace lineage {
namespace oplus_als {
namespace {
// See frameworks/base/services/core/jni/com_android_server_display_DisplayControl.cpp and
// frameworks/base/core/java/android/view/SurfaceControl.java
sp<IBinder> GetInternalDisplayToken() {
    const auto displayIds = SurfaceComposerClient::getPhysicalDisplayIds();
    return SurfaceComposerClient::getPhysicalDisplayToken(displayIds[0]);
}
};  // anonymous namespace

ndk::ScopedAStatus AreaCapture::getAreaBrightness(int32_t topX, int32_t topY, int32_t bottomX,
                                                  int32_t bottomY,
                                                  AreaRgbCaptureResult* _aidl_return) {
    Rect area{topX, topY, bottomX, bottomY};
    DisplayCaptureArgs displayCaptureArgs;
    displayCaptureArgs.captureArgs.pixelFormat = ::android::PIXEL_FORMAT_RGBA_8888;
    displayCaptureArgs.captureArgs.sourceCrop = toARect(area);
    displayCaptureArgs.captureArgs.captureSecureLayers = true;
    displayCaptureArgs.displayToken = GetInternalDisplayToken();
    displayCaptureArgs.width = area.getWidth();
    displayCaptureArgs.height = area.getHeight();

    sp<SyncScreenCaptureListener> captureListener = new SyncScreenCaptureListener();
    if (ScreenshotClient::captureDisplay(displayCaptureArgs, captureListener) !=
        ::android::NO_ERROR) {
        LOG(ERROR) << "Capture failed";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    auto captureResults = captureListener->waitForResults();
    if (!captureResults.fenceResult.ok()) {
        LOG(ERROR) << "Fence result error";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
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
