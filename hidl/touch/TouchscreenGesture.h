/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <vendor/lineage/touch/1.0/ITouchscreenGesture.h>
#include <map>

namespace vendor {
namespace lineage {
namespace touch {
namespace V1_0 {
namespace implementation {

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::vendor::lineage::touch::V1_0::Gesture;

class TouchscreenGesture : public ITouchscreenGesture {
  public:
    // Methods from ::vendor::lineage::touch::V1_0::ITouchscreenGesture follow.
    Return<void> getSupportedGestures(getSupportedGestures_cb resultCb) override;
    Return<bool> setGestureEnabled(const Gesture& gesture, bool enabled) override;

  private:
    // See: drivers/input/touchscreen/oplus_touchscreen_v2/touchpanel_common.h
    static constexpr int kGestureStartKey = 246;
    enum {
        kGestureUnknown,
        kGestureDoubleTap,
        kGestureUpVee,
        kGestureDownVee,
        kGestureLeftVee,
        kGestureRightVee,
        kGestureCircle,
        kGestureDoubleSwipe,
        kGestureLeftToRight,
        kGestureRightToLeft,
        kGestureUpToDown,
        kGestureDownToUp,
        kGestureM,
        kGestureW,
        kGestureFingerprintDown,
        kGestureFingerprintUp,
        kGestureSingleTap,
        kGestureHeart,
        kGestureS,
    };

    const std::map<int, std::string> kGestureNames = {
            {kGestureUnknown, "Unknown"},
            {kGestureDoubleTap, "Double tap"},
            {kGestureUpVee, "Down arrow"},
            {kGestureDownVee, "Up arrow"},
            {kGestureLeftVee, "Right arrow"},
            {kGestureRightVee, "Left arrow"},
            {kGestureCircle, "Letter O"},
            {kGestureDoubleSwipe, "Two fingers down swipe"},
            {kGestureLeftToRight, "One finger right swipe"},
            {kGestureRightToLeft, "One finger left swipe"},
            {kGestureUpToDown, "One finger down swipe"},
            {kGestureDownToUp, "One finger up swipe"},
            {kGestureM, "Letter M"},
            {kGestureW, "Letter W"},
            {kGestureFingerprintDown, "Fingerprint down"},
            {kGestureFingerprintUp, "Fingerprint up"},
            {kGestureSingleTap, "Single tap"},
            {kGestureHeart, "Heart"},
            {kGestureS, "Letter S"},
    };

    template <typename H, typename... T>
    static constexpr int makeBitField(H head, T... tail) {
        return ((1 << head) | ... | (1 << tail));
    }
    static const int kSupportedGestures;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
