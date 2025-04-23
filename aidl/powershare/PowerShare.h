/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/powershare/BnPowerShare.h>
#include <aidl/vendor/lineage/powershare/IPowerShare.h>

namespace aidl::vendor::lineage::powershare {

class PowerShare : public BnPowerShare {
  public:
    ndk::ScopedAStatus getMinBattery(int32_t* _aidl_return) override;
    ndk::ScopedAStatus isEnabled(bool* _aidl_return) override;
    ndk::ScopedAStatus setEnabled(bool in_enable, bool* _aidl_return) override;
    ndk::ScopedAStatus setMinBattery(int32_t in_minBattery, int32_t* _aidl_return) override;
};

}  // namespace aidl::vendor::lineage::powershare
