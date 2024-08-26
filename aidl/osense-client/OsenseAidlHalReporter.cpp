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

ndk::ScopedAStatus OsenseAidlHalReporter::checkAccessPermission(const std::string& identity,
                                                                int32_t* _aidl_return) {
    *_aidl_return = 1;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OsenseAidlHalReporter::osenseClrSceneAction(const std::string& identity,
                                                               int64_t request) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OsenseAidlHalReporter::osenseSetNotification(
        const std::string& identity, const OsenseAidlHalNotifyRequest& osenseHalNotifyRequest) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OsenseAidlHalReporter::osenseSetSceneAction(
        const std::string& identity, const OsenseAidlHalSaRequest& osenseHalSaRequest) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OsenseAidlHalReporter::osenseResetCtrlData(const std::string& identity) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OsenseAidlHalReporter::osenseSetCtrlData(const std::string& identity,
                                                            const OsenseControlInfo& ctrldata) {
    return ndk::ScopedAStatus::ok();
}

}  // namespace client
}  // namespace osense
}  // namespace hardware
}  // namespace oplus
}  // namespace vendor
}  // namespace aidl
