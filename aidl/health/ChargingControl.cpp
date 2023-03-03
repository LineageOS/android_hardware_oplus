/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ChargingControl.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android/binder_status.h>
#include <fstream>

#define LOG_TAG "vendor.lineage.health-service.oplus"

namespace aidl {
namespace vendor {
namespace lineage {
namespace health {

static const std::vector<ChargingEnabledNode> kChargingEnabledNodes = {
    {"/sys/class/oplus_chg/battery/mmi_charging_enable", "1", "0"},
};

static bool fileExists(const std::string& path) {
    std::fstream fs(path);
    return fs.is_open();
}

ChargingControl::ChargingControl() : mChargingEnabledNode(nullptr) {
    for (const auto& node : kChargingEnabledNodes) {
        if (!fileExists(node.path)) {
            continue;
        }

        mChargingEnabledNode = &node;
        break;
    }
}

ndk::ScopedAStatus ChargingControl::getChargingEnabled(bool* _aidl_return) {
    if (!mChargingEnabledNode) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    std::fstream chargingEnabledFile(mChargingEnabledNode->path);

    if (!chargingEnabledFile.is_open()) {
        LOG(ERROR) << "Failed to open charging enabled node";
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    std::string chargingEnabledResult;
    chargingEnabledFile >> chargingEnabledResult;

    if (chargingEnabledFile.fail()) {
        LOG(ERROR) << "Failed to read current charging enabled value";
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    if (chargingEnabledResult == mChargingEnabledNode->value_true) {
        *_aidl_return = true;
    } else if (chargingEnabledResult == mChargingEnabledNode->value_false) {
        *_aidl_return = false;
    } else {
        LOG(ERROR) << "Unknown value " << chargingEnabledResult;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus ChargingControl::setChargingEnabled(bool enabled) {
    if (!mChargingEnabledNode) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    std::fstream chargingEnabledFile(mChargingEnabledNode->path);

    if (!chargingEnabledFile.is_open()) {
        LOG(ERROR) << "Failed to open charging enabled node";
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    chargingEnabledFile << (enabled
                            ? mChargingEnabledNode->value_true
                            : mChargingEnabledNode->value_false);
    if (chargingEnabledFile.fail()) {
        LOG(ERROR) << "Failed to write to charging enable node: " << strerror(errno);
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace health
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
