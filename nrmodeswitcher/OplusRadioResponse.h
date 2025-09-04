/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/oplus/hardware/radio/BnOplusRadioResponse.h>

namespace aidl::vendor::oplus::hardware::radio {

class OplusRadioResponse : public BnOplusRadioResponse {
  public:
    explicit OplusRadioResponse(int32_t& result);
    ndk::ScopedAStatus setCallbackExtResponse() override;
    ndk::ScopedAStatus setNrModeResponse(const OplusRadioResponseInfo& info,
                                         int32_t result) override;
    ndk::ScopedAStatus getNrModeResponse(const OplusRadioResponseInfo& info,
                                         int32_t result) override;
    ndk::ScopedAStatus setModemErrorFatalResponse(const OplusRadioResponseInfo& info) override;
    ndk::ScopedAStatus setVoNrEnabledResponse(const OplusRadioResponseInfo& info) override;
    ndk::ScopedAStatus getVoNrEnabledResponse(const OplusRadioResponseInfo& info,
                                              bool result) override;
    ndk::ScopedAStatus setEccListResponse(const OplusRadioResponseInfo& info,
                                          const std::string& result) override;
    ndk::ScopedAStatus SetHsrModeForListeningResponse(const OplusRadioResponseInfo& info,
                                                      int32_t result) override;
    ndk::ScopedAStatus SetLogIdForListeningResponse(const OplusRadioResponseInfo& info,
                                                    int32_t result) override;
    ndk::ScopedAStatus setSlowStartResponse(const OplusRadioResponseInfo& info) override;
    ndk::ScopedAStatus setCtVolteModeResponse(const OplusRadioResponseInfo& info) override;
    ndk::ScopedAStatus simlockReqResponse(const OplusRadioResponseInfo& info,
                                          const std::vector<uint8_t>& result) override;
    ndk::ScopedAStatus updateRegionlockBlobResponse(const OplusRadioResponseInfo& info,
                                                    const std::vector<uint8_t>& status) override;
    ndk::ScopedAStatus updateRegionlockStatusResponse(const OplusRadioResponseInfo& info,
                                                      const std::vector<uint8_t>& status) override;
    ndk::ScopedAStatus getRegionlockStatusResponse(const OplusRadioResponseInfo& info,
                                                   const std::vector<uint8_t>& status) override;
    ndk::ScopedAStatus setNwCongestionCfgResponse(const OplusRadioResponseInfo& info,
                                                  const std::vector<uint8_t>& status) override;
    ndk::ScopedAStatus setModemEsimStatusResponse(const OplusRadioResponseInfo& info) override;
    ndk::ScopedAStatus getSimlockOperatorIdResponse(const OplusRadioResponseInfo& info,
                                                    int32_t result) override;
    ndk::ScopedAStatus getSimlockFeestateResponse(const OplusRadioResponseInfo& info,
                                                  int32_t result) override;
    ndk::ScopedAStatus setSimlockFeestateResponse(const OplusRadioResponseInfo& info) override;
    ndk::ScopedAStatus getSimlockUnlockStateResponse(const OplusRadioResponseInfo& info,
                                                     int32_t result) override;
    ndk::ScopedAStatus setSimlockFactoryResetTimeResponse(
            const OplusRadioResponseInfo& info) override;
    ndk::ScopedAStatus getSimlockFactoryResetTimeResponse(
            const OplusRadioResponseInfo& info, const std::vector<uint8_t>& status) override;
    ndk::ScopedAStatus setSimlockActivateTimeResponse(const OplusRadioResponseInfo& info) override;
    ndk::ScopedAStatus getSimlockActivateTimeResponse(const OplusRadioResponseInfo& info,
                                                      const std::vector<uint8_t>& status) override;
    ndk::ScopedAStatus getSimlockFeatureResponse(const OplusRadioResponseInfo& info,
                                                 const std::vector<uint8_t>& feature) override;
    ndk::ScopedAStatus getSimlockLockMarkResponse(const OplusRadioResponseInfo& info,
                                                  const std::vector<uint8_t>& lockmark) override;
    ndk::ScopedAStatus getSimlockDeviceLockStateResponse(const OplusRadioResponseInfo& info,
                                                         int32_t lockstate) override;
    ndk::ScopedAStatus getSimlockDeviceLockinfoResponse(
            const OplusRadioResponseInfo& info, const std::vector<uint8_t>& lockinfo) override;
    ndk::ScopedAStatus getSimlockVersionInfoResponse(const OplusRadioResponseInfo& info,
                                                     const std::vector<uint8_t>& version) override;
    ndk::ScopedAStatus getSimlockMaxRetryResponse(const OplusRadioResponseInfo& info,
                                                  int32_t result) override;
    ndk::ScopedAStatus getSimlockCurrentRetryResponse(const OplusRadioResponseInfo& info,
                                                      int32_t result) override;
    ndk::ScopedAStatus setPsDetachAttachActionResponse(const OplusRadioResponseInfo& info) override;
    ndk::ScopedAStatus unlockRegionlockResponse(const OplusRadioResponseInfo& info,
                                                int32_t result) override;
    ndk::ScopedAStatus updateRegionlockKeyResponse(const OplusRadioResponseInfo& info,
                                                   int32_t result) override;
    ndk::ScopedAStatus getRegionlockSettingDataResponse(
            const OplusRadioResponseInfo& info, const std::vector<uint8_t>& result) override;
    ndk::ScopedAStatus setRegionlockSettingDataResponse(const OplusRadioResponseInfo& info,
                                                        int32_t result) override;
    ndk::ScopedAStatus setSimlockOfflineLockResponse(const OplusRadioResponseInfo& info) override;
    ndk::ScopedAStatus getSimlockOfflineLockResponse(
            const OplusRadioResponseInfo& info, const std::vector<int32_t>& retryCount) override;

  private:
    int32_t& result_;
};

};  // namespace aidl::vendor::oplus::hardware::radio
