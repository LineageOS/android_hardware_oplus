/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Urcc.h"

#include <android-base/logging.h>

#define LOG_TAG "vendor.oplus.hardware.urcc-service"

namespace aidl {
namespace vendor {
namespace oplus {
namespace hardware {
namespace urcc {

ndk::ScopedAStatus Urcc::urccInit() {
    LOG(INFO) << __func__;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Urcc::urccResCtlRequest(const UrccRequestParcel& mUrccRequestParcel,
                                           int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Urcc::urccResCtlRelease(int32_t mhandle, int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Urcc::urccResStateRequest(const UrccRequestParcel& mUrccRequestParcel,
                                             std::vector<UrccRequestData>* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Urcc::urccResListeningRegister(
        const UrccRequestParcel& mUrccRequestParcel,
        const std::shared_ptr<IUrccCallback>& urccCallback, int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Urcc::urccResListeningUnRegister(int32_t mhandle, int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Urcc::urccPropertyGet(const std::string& name, std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Urcc::urccPropertySet(const std::string& name, const std::string& value,
                                         int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Urcc::urccThermalListeningRegister(
        const std::vector<int32_t>& types, const std::shared_ptr<IUrccCallback>& urccCallback,
        int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Urcc::urccThermalListeningUnRegister(int32_t mhandle, int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Urcc::uahNotifyExt(int32_t src, int32_t type, const std::vector<int32_t>& args,
                                      int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Urcc::setRelatedSysInfo(int32_t cmd, const std::vector<uint8_t>& info,
                                           int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Urcc::urccRuleCtl(int32_t ruleId, int32_t status,
                                     const std::vector<UrccRequestData>& ruleData,
                                     int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Urcc::uahResCtlRequestBypass(const UrccRequestParcel& mRequestParcel) {
    LOG(INFO) << __func__;
    return ndk::ScopedAStatus::ok();
}

}  // namespace urcc
}  // namespace hardware
}  // namespace oplus
}  // namespace vendor
}  // namespace aidl
