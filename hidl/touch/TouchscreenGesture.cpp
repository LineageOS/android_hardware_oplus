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

#define LOG_TAG "vendor.lineage.touch@1.0-service.oplus"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>
#include <android/binder_manager.h>

#include <TouchscreenGestureConfig.h>

using ::android::base::Trim;

namespace {}  // anonymous namespace

namespace vendor {
namespace lineage {
namespace touch {
namespace V1_0 {
namespace implementation {

Return<void> TouchscreenGesture::getSupportedGestures(getSupportedGestures_cb resultCb) {
    std::vector<Gesture> gestures;

    for (const auto& [id, name] : kGestureNames) {
        if (kSupportedGestures & (1 << id)) {
            gestures.push_back({static_cast<int>(gestures.size()), name, kGestureStartKey + id});
        }
    }

    resultCb(gestures);

    return Void();
}

Return<bool> TouchscreenGesture::setGestureEnabled(const Gesture& gesture, bool enabled) {
    std::string tmp = "OPTG";
    int contents = 0;
    int result;

    // Connect to IOplusTouch
    const std::string instance = std::string() + IOplusTouch::descriptor + "/default";
    mTouchService = IOplusTouch::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(instance.c_str())));

    // Read current value
    mTouchService->touchReadNodeFile(0, 21, &tmp);
    contents = std::stoi(tmp, nullptr, 16);

    // Manipulate value
    if (enabled) {
        contents |= (1 << (gesture.keycode - kGestureStartKey));
    } else {
        contents &= ~(1 << (gesture.keycode - kGestureStartKey));
    }

    // Always keep gestures enabled
    mTouchService->touchWriteNodeFile(0, 1, "1", &result);
    // Set gesture type
    mTouchService->touchWriteNodeFile(0, 21, std::to_string(contents), &result);

    return true;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
