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
    ndk::ScopedAStatus checkAccessPermission(const std::string& identity,
                                             int32_t* _aidl_return) final;
    ndk::ScopedAStatus osenseClrSceneAction(const std::string& identity, int64_t request) final;
    ndk::ScopedAStatus osenseSetNotification(
            const std::string& identity,
            const OsenseAidlHalNotifyRequest& osenseHalNotifyRequest) final;
    ndk::ScopedAStatus osenseSetSceneAction(const std::string& identity,
                                            const OsenseAidlHalSaRequest& osenseHalSaRequest) final;
    ndk::ScopedAStatus osenseResetCtrlData(const std::string& identity) final;
    ndk::ScopedAStatus osenseSetCtrlData(const std::string& identity,
                                         const OsenseControlInfo& ctrldata) final;
};

}  // namespace client
}  // namespace osense
}  // namespace hardware
}  // namespace oplus
}  // namespace vendor
}  // namespace aidl
