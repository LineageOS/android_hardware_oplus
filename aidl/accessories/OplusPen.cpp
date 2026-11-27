/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "OplusPen"

#include "OplusPen.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include <vector>

using ::android::base::Join;
using ::android::base::ParseUint;
using ::android::base::ReadFileToString;
using ::android::base::StartsWith;
using ::android::base::StringPrintf;
using ::android::base::Trim;

namespace aidl {
namespace vendor {
namespace lineage {
namespace accessories {

namespace {

const std::string kPencilDevPath = "/devices/virtual/oplus_wireless/pencil";
const std::string kPencilPresentPath = "/sys" + kPencilDevPath + "/present";
const std::string kPencilAddressPath = "/sys" + kPencilDevPath + "/ble_mac_addr";
const std::string kPencilUeventMatch = "DEVPATH=" + kPencilDevPath;

constexpr const char* kPencilStatusKey = "pencil_status";
constexpr const char* kPencilAddressKey = "pencil_addr";
constexpr const char* kPencilAttached = "1";

constexpr uint64_t kMaxAddress = 0xFFFFFFFFFFFF;
constexpr int kAddressOctets = 6;

std::string formatAddress(uint64_t address) {
    std::vector<std::string> octets;
    for (int octet = kAddressOctets - 1; octet >= 0; octet--) {
        octets.push_back(
                StringPrintf("%02X", static_cast<unsigned>((address >> (octet * 8)) & 0xFF)));
    }
    return Join(octets, ':');
}

// sysfs reports "0x<address>_(Time:<ms>ms)" while uevents carry the bare hex number
std::optional<std::string> parseAddress(const std::string& value) {
    std::string hex = value.substr(0, value.find('_'));
    if (!StartsWith(hex, "0x")) {
        hex.insert(0, "0x");
    }

    uint64_t address;
    if (!ParseUint(hex, &address, kMaxAddress) || address == 0) {
        return std::nullopt;
    }
    return formatAddress(address);
}

}  // anonymous namespace

OplusPen::OplusPen()
    : mListener(kPencilUeventMatch,
                [this](const UeventListener::Environment& environment) { onUevent(environment); }) {
    mAccessoryInfo = {
            .id = "active_pen:internal:1",
            .type = AccessoryType::ACTIVE_PEN,
            .isPersistent = true,
            .displayName = "Pen",
            .supportsToggling = false,
            .supportsBatteryQuerying = false,
            .isCharging = false,
            .dataLink = std::nullopt,
    };
    mAccessoryStatus = AccessoryStatus::ENABLED;

    std::lock_guard lock(mMutex);

    // Uevents received while reading the initial state are applied after it
    mListener.start();

    std::string present, address;
    if (!ReadFileToString(kPencilPresentPath, &present) ||
        !ReadFileToString(kPencilAddressPath, &address)) {
        PLOG(ERROR) << "Failed to read the pen state from " << kPencilDevPath;
        return;
    }
    updateLocked(Trim(present) == kPencilAttached, parseAddress(address));
}

::ndk::ScopedAStatus OplusPen::setEnabled(bool /*enabled*/) {
    return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

void OplusPen::onUevent(const UeventListener::Environment& environment) {
    auto status = environment.find(kPencilStatusKey);
    if (status == environment.end()) {
        return;
    }
    auto address = environment.find(kPencilAddressKey);

    std::lock_guard lock(mMutex);
    updateLocked(status->second == kPencilAttached,
                 address != environment.end() ? parseAddress(address->second) : std::nullopt);
}

void OplusPen::updateLocked(bool isAttached, const std::optional<std::string>& address) {
    bool isAddressChanged = isAttached && address &&
                            (!mAccessoryInfo.dataLink || mAccessoryInfo.dataLink->id != *address);
    if (isAttached == mAccessoryInfo.isCharging && !isAddressChanged) {
        return;
    }

    mAccessoryInfo.isCharging = isAttached;
    if (isAddressChanged) {
        mAccessoryInfo.dataLink = DataLinkInfo{
                .type = DataLinkType::BLUETOOTH,
                .id = *address,
        };
    }

    LOG(INFO) << "Pen " << (isAttached ? "attached" : "detached")
              << (mAccessoryInfo.dataLink ? ", address " + mAccessoryInfo.dataLink->id : "");
    onAccessoryInfoUpdated();
}

}  // namespace accessories
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
