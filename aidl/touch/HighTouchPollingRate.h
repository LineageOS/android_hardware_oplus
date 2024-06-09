/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ifdef USE_OPLUSTOUCH
#include <aidl/vendor/oplus/hardware/touch/IOplusTouch.h>
#endif
#include <aidl/vendor/lineage/touch/BnHighTouchPollingRate.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

#ifdef USE_OPLUSTOUCH
using ::aidl::vendor::oplus::hardware::touch::IOplusTouch;
#endif

class HighTouchPollingRate : public BnHighTouchPollingRate {
  public:
    ndk::ScopedAStatus getEnabled(bool* _aidl_return) override;
    ndk::ScopedAStatus setEnabled(bool enabled) override;
};

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
