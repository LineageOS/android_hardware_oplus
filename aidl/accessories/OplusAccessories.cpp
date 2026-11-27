/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "OplusAccessories.h"

#include "OplusPen.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace accessories {

OplusAccessories::OplusAccessories() {
    addAccessory(::ndk::SharedRefBase::make<OplusPen>());
}

}  // namespace accessories
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
