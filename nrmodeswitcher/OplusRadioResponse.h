/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/oplus/hardware/radio/BnOplusRadioResponse.h>

namespace aidl::vendor::oplus::hardware::radio {

class OplusRadioResponse : public BnOplusRadioResponse {
  public:
    explicit OplusRadioResponse(int32_t& in_result);
    ndk::ScopedAStatus setCallbackExtResponse() override;
    ndk::ScopedAStatus setNrModeResponse(const OplusRadioResponseInfo& in_info,
                                         int32_t in_result) override;
    ndk::ScopedAStatus getNrModeResponse(const OplusRadioResponseInfo& in_info,
                                         int32_t in_result) override;
    ndk::ScopedAStatus setModemErrorFatalResponse(const OplusRadioResponseInfo& in_info) override;
    ndk::ScopedAStatus setVoNrEnabledResponse(const OplusRadioResponseInfo& in_info) override;
    ndk::ScopedAStatus getVoNrEnabledResponse(const OplusRadioResponseInfo& in_info,
                                              bool in_result) override;
    ndk::ScopedAStatus setEccListResponse(const OplusRadioResponseInfo& in_info,
                                          const std::string& in_result) override;
    ndk::ScopedAStatus SetHsrModeForListeningResponse(const OplusRadioResponseInfo& in_info,
                                                      int32_t in_result) override;
    ndk::ScopedAStatus SetLogIdForListeningResponse(const OplusRadioResponseInfo& in_info,
                                                    int32_t in_result) override;
    ndk::ScopedAStatus setSlowStartResponse(const OplusRadioResponseInfo& in_info) override;
    ndk::ScopedAStatus setCtVolteModeResponse(const OplusRadioResponseInfo& in_info) override;
    ndk::ScopedAStatus simlockReqResponse(const OplusRadioResponseInfo& in_info,
                                          const std::vector<uint8_t>& in_result) override;
    ndk::ScopedAStatus updateRegionlockBlobResponse(const OplusRadioResponseInfo& in_info,
                                                    const std::vector<uint8_t>& in_status) override;
    ndk::ScopedAStatus updateRegionlockStatusResponse(
            const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_status) override;
    ndk::ScopedAStatus getRegionlockStatusResponse(const OplusRadioResponseInfo& in_info,
                                                   const std::vector<uint8_t>& in_status) override;
    ndk::ScopedAStatus setNwCongestionCfgResponse(const OplusRadioResponseInfo& in_info,
                                                  const std::vector<uint8_t>& in_status) override;
    ndk::ScopedAStatus setModemEsimStatusResponse(const OplusRadioResponseInfo& in_info) override;
    ndk::ScopedAStatus getSimlockOperatorIdResponse(const OplusRadioResponseInfo& in_info,
                                                    int32_t in_result) override;
    ndk::ScopedAStatus getSimlockFeestateResponse(const OplusRadioResponseInfo& in_info,
                                                  int32_t in_result) override;
    ndk::ScopedAStatus setSimlockFeestateResponse(const OplusRadioResponseInfo& in_info) override;
    ndk::ScopedAStatus getSimlockUnlockStateResponse(const OplusRadioResponseInfo& in_info,
                                                     int32_t in_result) override;
    ndk::ScopedAStatus setSimlockFactoryResetTimeResponse(
            const OplusRadioResponseInfo& in_info) override;
    ndk::ScopedAStatus getSimlockFactoryResetTimeResponse(
            const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_status) override;
    ndk::ScopedAStatus setSimlockActivateTimeResponse(
            const OplusRadioResponseInfo& in_info) override;
    ndk::ScopedAStatus getSimlockActivateTimeResponse(
            const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_status) override;
    ndk::ScopedAStatus getSimlockFeatureResponse(const OplusRadioResponseInfo& in_info,
                                                 const std::vector<uint8_t>& in_feature) override;
    ndk::ScopedAStatus getSimlockLockMarkResponse(const OplusRadioResponseInfo& in_info,
                                                  const std::vector<uint8_t>& in_lockmark) override;
    ndk::ScopedAStatus getSimlockDeviceLockStateResponse(const OplusRadioResponseInfo& in_info,
                                                         int32_t in_lockstate) override;
    ndk::ScopedAStatus getSimlockDeviceLockinfoResponse(
            const OplusRadioResponseInfo& in_info,
            const std::vector<uint8_t>& in_lockinfo) override;
    ndk::ScopedAStatus getSimlockVersionInfoResponse(
            const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_version) override;
    ndk::ScopedAStatus getSimlockMaxRetryResponse(const OplusRadioResponseInfo& in_info,
                                                  int32_t in_result) override;
    ndk::ScopedAStatus getSimlockCurrentRetryResponse(const OplusRadioResponseInfo& in_info,
                                                      int32_t in_result) override;
    ndk::ScopedAStatus setPsDetachAttachActionResponse(
            const OplusRadioResponseInfo& in_info) override;
    ndk::ScopedAStatus unlockRegionlockResponse(const OplusRadioResponseInfo& in_info,
                                                int32_t in_result) override;
    ndk::ScopedAStatus updateRegionlockKeyResponse(const OplusRadioResponseInfo& in_info,
                                                   int32_t in_result) override;
    ndk::ScopedAStatus getRegionlockSettingDataResponse(
            const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_result) override;
    ndk::ScopedAStatus setRegionlockSettingDataResponse(const OplusRadioResponseInfo& in_info,
                                                        int32_t in_result) override;
    ndk::ScopedAStatus setSimlockOfflineLockResponse(
            const OplusRadioResponseInfo& in_info) override;
    ndk::ScopedAStatus getSimlockOfflineLockResponse(
            const OplusRadioResponseInfo& in_info,
            const std::vector<int32_t>& in_retryCount) override;

  private:
    int32_t& in_result_;
};

};  // namespace aidl::vendor::oplus::hardware::radio
