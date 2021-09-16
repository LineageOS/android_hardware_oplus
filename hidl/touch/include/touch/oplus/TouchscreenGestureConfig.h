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
    {0, {253, "Two fingers down swipe"}},
    {1, {249, "Up arrow"}},
    {2, {248, "Down arrow"}},
    {3, {251, "Left arrow"}},
    {4, {250, "Right arrow"}},
    {5, {257, "One finger up swipe"}},
    {6, {256, "One finger down swipe"}},
    {7, {255, "One finger left swipe"}},
    {8, {254, "One finger right swipe"}},
    {9, {258, "Letter M"}},
    {10, {252, "Letter O"}},
    {11, {264, "Letter S"}},
    {12, {259, "Letter W"}},
    {13, {262, "Single Tap"}},
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
