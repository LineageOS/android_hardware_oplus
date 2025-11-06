/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "CommonDcsHalService.h"

namespace vendor::oplus::hardware::commondcs::V1_0::implementation {

Return<int32_t> CommonDcsHalService::notifyMsgToCommonDcs(const hidl_vec<::vendor::oplus::hardware::commondcs::V1_0::StringPair>& data, const hidl_string& logTag, const hidl_string& eventId) {
    return 0;
}

} // namespace vendor::oplus::hardware::commondcs::V1_0::implementation
