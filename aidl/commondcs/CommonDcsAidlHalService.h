/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/oplus/hardware/commondcs/BnCommonDcsAidlHalService.h>

namespace aidl {
namespace vendor {
namespace oplus {
namespace hardware {
namespace commondcs {

struct CommonDcsAidlHalService : public BnCommonDcsAidlHalService {
    ndk::ScopedAStatus notifyMsgToCommonDcs(const std::vector<StringPair>& data,
                                            const std::string& logTag, const std::string& eventId,
                                            int32_t* _aidl_return) final;
};

}  // namespace commondcs
}  // namespace hardware
}  // namespace oplus
}  // namespace vendor
}  // namespace aidl
