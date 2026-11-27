/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/accessories/Accessory.h>

#include <mutex>
#include <optional>
#include <string>

#include "UeventListener.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace accessories {

class OplusPen : public Accessory {
  public:
    OplusPen();
    OplusPen(const OplusPen&) = delete;

    ::ndk::ScopedAStatus setEnabled(bool enabled) override;

  private:
    void onUevent(const UeventListener::Environment& environment);
    void updateLocked(bool isAttached, const std::optional<std::string>& address);

    std::mutex mMutex;
    UeventListener mListener;
};

}  // namespace accessories
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
