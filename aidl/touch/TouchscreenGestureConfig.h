/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "TouchscreenGesture.h"

namespace aidl::vendor::lineage::touch {

const int TouchscreenGesture::kSupportedGestures = makeBitField(
        kGestureUpVee, kGestureDownVee, kGestureLeftVee, kGestureRightVee, kGestureCircle,
        kGestureDoubleSwipe, kGestureLeftToRight, kGestureRightToLeft, kGestureUpToDown,
        kGestureDownToUp, kGestureM, kGestureW, kGestureSingleTap, kGestureS);

}  // namespace aidl::vendor::lineage::touch
