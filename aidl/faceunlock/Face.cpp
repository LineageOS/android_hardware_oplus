/*
 * Copyright (C) 2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Face.h"

namespace aidl {
namespace android {
namespace hardware {
namespace biometrics {
namespace face {

ndk::ScopedAStatus Face::getSensorProps(std::vector<SensorProps>* /*_aidl_return*/) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Face::createSession(int32_t /*sensorId*/, int32_t /*userId*/, const std::shared_ptr<ISessionCallback>& /*cb*/, std::shared_ptr<ISession>* /*_aidl_return*/) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

} // namespace face
} // namespace biometrics
} // namespace hardware
} // namespace android
} // namespace aidl
