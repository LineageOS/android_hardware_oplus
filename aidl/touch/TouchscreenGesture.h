/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/touch/BnTouchscreenGesture.h>
#include <map>

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

class TouchscreenGesture : public BnTouchscreenGesture {
  public:
    ndk::ScopedAStatus getSupportedGestures(std::vector<Gesture>* _aidl_return) override;
    ndk::ScopedAStatus setGestureEnabled(const Gesture& gesture, bool enabled) override;

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

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
