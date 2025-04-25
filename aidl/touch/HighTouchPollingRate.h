/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/touch/BnHighTouchPollingRate.h>

namespace aidl::vendor::lineage::touch {

class HighTouchPollingRate : public BnHighTouchPollingRate {
  public:
    ndk::ScopedAStatus setEnabled(bool enabled) override;
    ndk::ScopedAStatus getEnabled(bool* _aidl_return) override;
};

}  // namespace aidl::vendor::lineage::touch
