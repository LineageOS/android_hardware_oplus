/*
 * Copyright (C) 2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/biometrics/face/BnFace.h>

using ::aidl::android::hardware::biometrics::face::ISession;
using ::aidl::android::hardware::biometrics::face::ISessionCallback;
using ::aidl::android::hardware::biometrics::face::SensorProps;

namespace aidl {
namespace android {
namespace hardware {
namespace biometrics {
namespace face {

class Face : public BnFace {
public:
    ndk::ScopedAStatus getSensorProps(std::vector<SensorProps>* _aidl_return) override;
    ndk::ScopedAStatus createSession(int32_t sensorId, int32_t userId, const std::shared_ptr<ISessionCallback>& cb, std::shared_ptr<ISession>* _aidl_return) override;
};

} // namespace face
} // namespace biometrics
} // namespace hardware
} // namespace android
} // namespace aidl
