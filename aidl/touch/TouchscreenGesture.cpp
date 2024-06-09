/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.oplus"

#include <android-base/file.h>
#include <android-base/strings.h>

#include <TouchscreenGestureConfig.h>

#ifdef USE_OPLUSTOUCH
#include <android/binder_manager.h>
#else
using ::android::base::ReadFileToString;
using ::android::base::Trim;
using ::android::base::WriteStringToFile;

namespace {

constexpr const char* kGestureEnableIndepPath = "/proc/touchpanel/double_tap_enable_indep";

}  // anonymous namespace
#endif

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

ndk::ScopedAStatus TouchscreenGesture::getSupportedGestures(std::vector<Gesture>* _aidl_return) {
    std::vector<Gesture> gestures;

    for (const auto& [id, name] : kGestureNames) {
        if (kSupportedGestures & (1 << id)) {
            gestures.push_back({static_cast<int>(gestures.size()), name, kGestureStartKey + id});
        }
    }

    *_aidl_return = gestures;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus TouchscreenGesture::setGestureEnabled(const Gesture& gesture, bool enabled) {
    std::string tmp;
    int contents = 0;
#ifdef USE_OPLUSTOUCH
    int result;

    // Connect to IOplusTouch
    const std::string instance = std::string() + IOplusTouch::descriptor + "/default";
    std::shared_ptr<IOplusTouch> oplusTouch = IOplusTouch::fromBinder(
            ndk::SpAIBinder(AServiceManager_waitForService(instance.c_str())));

    oplusTouch->touchReadNodeFile(0, 21, &tmp);
    contents = std::stoi(tmp, nullptr, 16);
#else
    if (ReadFileToString(kGestureEnableIndepPath, &tmp)) {
        contents = std::stoi(Trim(tmp), nullptr, 16);
    } else {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
#endif

    if (enabled) {
        contents |= (1 << (gesture.keycode - kGestureStartKey));
    } else {
        contents &= ~(1 << (gesture.keycode - kGestureStartKey));
    }

#ifdef USE_OPLUSTOUCH
    oplusTouch->touchWriteNodeFile(0, 1, "1", &result);
    oplusTouch->touchWriteNodeFile(0, 21, std::to_string(contents), &result);
#else
    if (!WriteStringToFile(std::to_string(contents), kGestureEnableIndepPath, true)) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
#endif

    return ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
