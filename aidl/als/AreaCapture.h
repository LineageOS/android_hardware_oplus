/*
 * SPDX-FileCopyrightText: 2021-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/oplus_als/BnAreaCapture.h>

namespace aidl {
namespace vendor {
namespace lineage {
namespace oplus_als {

class AreaCapture : public BnAreaCapture {
  public:
    ndk::ScopedAStatus getAreaBrightness(int32_t topX, int32_t topY, int32_t bottomX,
                                         int32_t bottomY,
                                         AreaRgbCaptureResult* _aidl_return) override;
};

}  // namespace oplus_als
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
