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

const std::map<int32_t, TouchscreenGesture::GestureInfo> TouchscreenGesture::kGestureInfoMap = {
        {0, {253, "Two fingers down swipe", 1 << 7}},
        {1, {249, "Up arrow", 1 << 3}},
        {2, {248, "Down arrow", 1 << 2}},
        {3, {251, "Left arrow", 1 << 5}},
        {4, {250, "Right arrow", 1 << 4}},
        {5, {257, "One finger up swipe", 1 << 11}},
        {6, {256, "One finger down swipe", 1 << 10}},
        {7, {255, "One finger left swipe", 1 << 9}},
        {8, {254, "One finger right swipe", 1 << 8}},
        {9, {258, "Letter M", 1 << 12}},
        {10, {252, "Letter O", 1 << 6}},
        {11, {264, "Letter S", 1 << 18}},
        {12, {259, "Letter W", 1 << 13}},
        {13, {262, "Single Tap", 1 << 16}},
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
