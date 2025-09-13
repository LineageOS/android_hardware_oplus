/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/oplus/hardware/urcc/BnUrcc.h>

namespace aidl {
namespace vendor {
namespace oplus {
namespace hardware {
namespace urcc {

class Urcc : public BnUrcc {
    ndk::ScopedAStatus urccInit() override;
    ndk::ScopedAStatus urccResCtlRequest(const UrccRequestParcel& mUrccRequestParcel,
                                         int32_t* _aidl_return) override;
    ndk::ScopedAStatus urccResCtlRelease(int32_t mhandle, int32_t* _aidl_return) override;
    ndk::ScopedAStatus urccResStateRequest(const UrccRequestParcel& mUrccRequestParcel,
                                           std::vector<UrccRequestData>* _aidl_return) override;
    ndk::ScopedAStatus urccResListeningRegister(const UrccRequestParcel& mUrccRequestParcel,
                                                const std::shared_ptr<IUrccCallback>& urccCallback,
                                                int32_t* _aidl_return) override;
    ndk::ScopedAStatus urccResListeningUnRegister(int32_t mhandle, int32_t* _aidl_return) override;
    ndk::ScopedAStatus urccPropertyGet(const std::string& name, std::string* _aidl_return) override;
    ndk::ScopedAStatus urccPropertySet(const std::string& name, const std::string& value,
                                       int32_t* _aidl_return) override;
    ndk::ScopedAStatus urccThermalListeningRegister(
            const std::vector<int32_t>& types, const std::shared_ptr<IUrccCallback>& urccCallback,
            int32_t* _aidl_return) override;
    ndk::ScopedAStatus urccThermalListeningUnRegister(int32_t mhandle,
                                                      int32_t* _aidl_return) override;
    ndk::ScopedAStatus uahNotifyExt(int32_t src, int32_t type, const std::vector<int32_t>& args,
                                    int32_t* _aidl_return) override;
    ndk::ScopedAStatus setRelatedSysInfo(int32_t cmd, const std::vector<uint8_t>& info,
                                         int32_t* _aidl_return) override;
    ndk::ScopedAStatus urccRuleCtl(int32_t ruleId, int32_t status,
                                   const std::vector<UrccRequestData>& ruleData,
                                   int32_t* _aidl_return) override;
    ndk::ScopedAStatus uahResCtlRequestBypass(const UrccRequestParcel& mRequestParcel) override;
};

}  // namespace urcc
}  // namespace hardware
}  // namespace oplus
}  // namespace vendor
}  // namespace aidl
