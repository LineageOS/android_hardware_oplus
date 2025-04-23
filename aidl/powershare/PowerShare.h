/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/powershare/BnPowerShare.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace powershare {

class PowerShare : public BnPowerShare {
  public:
    ndk::ScopedAStatus getMinBattery(int32_t* _aidl_return) override;
    ndk::ScopedAStatus isEnabled(bool* _aidl_return) override;
    ndk::ScopedAStatus setEnabled(bool enable) override;
    ndk::ScopedAStatus setMinBattery(int32_t minBattery) override;
};

}  // namespace powershare
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
