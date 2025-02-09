/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "CommonDcsAidlHalService.h"

namespace aidl {
namespace vendor {
namespace oplus {
namespace hardware {
namespace commondcs {

ndk::ScopedAStatus CommonDcsAidlHalService::notifyMsgToCommonDcs(
        const std::vector<StringPair>& data, const std::string& logTag, const std::string& eventId,
        int32_t* _aidl_return) {
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

}  // namespace commondcs
}  // namespace hardware
}  // namespace oplus
}  // namespace vendor
}  // namespace aidl
