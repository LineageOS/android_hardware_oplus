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

#include "TouchscreenGesture.h"

namespace vendor {
namespace lineage {
namespace touch {
namespace V1_0 {
namespace implementation {

const int TouchscreenGesture::kSupportedGestures =
        (1 << kGestureUpVee) | (1 << kGestureDownVee) | (1 << kGestureLeftVee) |
        (1 << kGestureRightVee) | (1 << kGestureCircle) | (1 << kGestureDoubleSwipe) |
        (1 << kGestureLeftToRight) | (1 << kGestureRightToLeft) | (1 << kGestureUpToDown) |
        (1 << kGestureDownToUp) | (1 << kGestureM) | (1 << kGestureW) | (1 << kGestureSingleTap) |
        (1 << kGestureS);

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
