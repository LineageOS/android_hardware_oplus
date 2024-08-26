/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/oplus/hardware/osense/client/BnOsenseAidlHalReporter.h>

namespace aidl {
namespace vendor {
namespace oplus {
namespace hardware {
namespace osense {
namespace client {

struct OsenseAidlHalReporter : public BnOsenseAidlHalReporter {
    ndk::ScopedAStatus checkAccessPermission(const std::string& in_identity,
                                             int32_t* _aidl_return) final;
    ndk::ScopedAStatus osenseClrSceneAction(const std::string& in_identity,
                                            int64_t in_request) final;
    ndk::ScopedAStatus osenseSetNotification(
            const std::string& in_identity,
            const OsenseAidlHalNotifyRequest& in_osenseHalNotifyRequest) final;
    ndk::ScopedAStatus osenseSetSceneAction(
            const std::string& in_identity,
            const OsenseAidlHalSaRequest& in_osenseHalSaRequest) final;
    ndk::ScopedAStatus osenseResetCtrlData(const std::string& in_identity) final;
    ndk::ScopedAStatus osenseSetCtrlData(const std::string& in_identity,
                                         const OsenseControlInfo& in_ctrldata) final;
};

}  // namespace client
}  // namespace osense
}  // namespace hardware
}  // namespace oplus
}  // namespace vendor
}  // namespace aidl
