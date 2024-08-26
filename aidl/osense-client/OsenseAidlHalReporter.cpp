/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "OsenseAidlHalReporter.h"

namespace aidl {
namespace vendor {
namespace oplus {
namespace hardware {
namespace osense {
namespace client {

ndk::ScopedAStatus OsenseAidlHalReporter::checkAccessPermission(const std::string& in_identity,
                                                                int32_t* _aidl_return) {
    *_aidl_return = 1;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OsenseAidlHalReporter::osenseClrSceneAction(const std::string& in_identity,
                                                               int64_t in_request) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OsenseAidlHalReporter::osenseSetNotification(
        const std::string& in_identity,
        const OsenseAidlHalNotifyRequest& in_osenseHalNotifyRequest) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OsenseAidlHalReporter::osenseSetSceneAction(
        const std::string& in_identity, const OsenseAidlHalSaRequest& in_osenseHalSaRequest) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OsenseAidlHalReporter::osenseResetCtrlData(const std::string& in_identity) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OsenseAidlHalReporter::osenseSetCtrlData(const std::string& in_identity,
                                                            const OsenseControlInfo& in_ctrldata) {
    return ndk::ScopedAStatus::ok();
}

}  // namespace client
}  // namespace osense
}  // namespace hardware
}  // namespace oplus
}  // namespace vendor
}  // namespace aidl
