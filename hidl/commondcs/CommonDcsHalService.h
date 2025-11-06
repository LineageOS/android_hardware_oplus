/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <vendor/oplus/hardware/commondcs/1.0/ICommonDcsHalService.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace vendor {
namespace oplus {
namespace hardware {
namespace commondcs {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;

struct CommonDcsHalService : public ICommonDcsHalService {
    Return<int32_t> notifyMsgToCommonDcs(const hidl_vec<::vendor::oplus::hardware::commondcs::V1_0::StringPair>& data, const hidl_string& logTag, const hidl_string& eventId) override;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace commondcs
}  // namespace hardware
}  // namespace oplus
}  // namespace vendor
