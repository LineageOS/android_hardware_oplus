/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "CommonDcsHalService.h"

namespace vendor {
namespace oplus {
namespace hardware {
namespace commondcs {
namespace V1_0 {
namespace implementation {

Return<int32_t> CommonDcsHalService::notifyMsgToCommonDcs(const hidl_vec<::vendor::oplus::hardware::commondcs::V1_0::StringPair>& data, const hidl_string& logTag, const hidl_string& eventId) {
    return 0;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace commondcs
}  // namespace hardware
}  // namespace oplus
}  // namespace vendor
