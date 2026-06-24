/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/touch/BnHighTouchPollingRate.h>
#include <aidl/vendor/oplus/hardware/touch/IOplusTouch.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

using aidl::vendor::oplus::hardware::touch::IOplusTouch;

class HighTouchPollingRate : public BnHighTouchPollingRate {
  public:
    explicit HighTouchPollingRate(std::shared_ptr<IOplusTouch> oplusTouch);

    ndk::ScopedAStatus getEnabled(bool* _aidl_return) override;
    ndk::ScopedAStatus setEnabled(bool enabled) override;

  private:
    std::string highestSupportedMode();

    std::shared_ptr<IOplusTouch> mOplusTouch;
    std::string mHighestSupportedMode;
};

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
