/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "OplusRadioResponse.h"

namespace aidl::vendor::oplus::hardware::radio {

OplusRadioResponse::OplusRadioResponse(int32_t& in_result) : in_result_(in_result) {}

ndk::ScopedAStatus OplusRadioResponse::setCallbackExtResponse() {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::setNrModeResponse(const OplusRadioResponseInfo& in_info,
                                                         int32_t in_result) {
    in_result_ = in_result;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getNrModeResponse(const OplusRadioResponseInfo& in_info,
                                                         int32_t in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::setModemErrorFatalResponse(
        const OplusRadioResponseInfo& in_info) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::setVoNrEnabledResponse(
        const OplusRadioResponseInfo& in_info) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getVoNrEnabledResponse(const OplusRadioResponseInfo& in_info,
                                                              bool in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::setEccListResponse(const OplusRadioResponseInfo& in_info,
                                                          const std::string& in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::SetHsrModeForListeningResponse(
        const OplusRadioResponseInfo& in_info, int32_t in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::SetLogIdForListeningResponse(
        const OplusRadioResponseInfo& in_info, int32_t in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::setSlowStartResponse(const OplusRadioResponseInfo& in_info) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::setCtVolteModeResponse(
        const OplusRadioResponseInfo& in_info) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::simlockReqResponse(const OplusRadioResponseInfo& in_info,
                                                          const std::vector<uint8_t>& in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::updateRegionlockBlobResponse(
        const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_status) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::updateRegionlockStatusResponse(
        const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_status) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getRegionlockStatusResponse(
        const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_status) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::setNwCongestionCfgResponse(
        const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_status) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::setModemEsimStatusResponse(
        const OplusRadioResponseInfo& in_info) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getSimlockOperatorIdResponse(
        const OplusRadioResponseInfo& in_info, int32_t in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getSimlockFeestateResponse(
        const OplusRadioResponseInfo& in_info, int32_t in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::setSimlockFeestateResponse(
        const OplusRadioResponseInfo& in_info) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getSimlockUnlockStateResponse(
        const OplusRadioResponseInfo& in_info, int32_t in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::setSimlockFactoryResetTimeResponse(
        const OplusRadioResponseInfo& in_info) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getSimlockFactoryResetTimeResponse(
        const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_status) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::setSimlockActivateTimeResponse(
        const OplusRadioResponseInfo& in_info) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getSimlockActivateTimeResponse(
        const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_status) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getSimlockFeatureResponse(
        const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_feature) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getSimlockLockMarkResponse(
        const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_lockmark) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getSimlockDeviceLockStateResponse(
        const OplusRadioResponseInfo& in_info, int32_t in_lockstate) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getSimlockDeviceLockinfoResponse(
        const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_lockinfo) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getSimlockVersionInfoResponse(
        const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_version) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getSimlockMaxRetryResponse(
        const OplusRadioResponseInfo& in_info, int32_t in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getSimlockCurrentRetryResponse(
        const OplusRadioResponseInfo& in_info, int32_t in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::setPsDetachAttachActionResponse(
        const OplusRadioResponseInfo& in_info) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::unlockRegionlockResponse(
        const OplusRadioResponseInfo& in_info, int32_t in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::updateRegionlockKeyResponse(
        const OplusRadioResponseInfo& in_info, int32_t in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getRegionlockSettingDataResponse(
        const OplusRadioResponseInfo& in_info, const std::vector<uint8_t>& in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::setRegionlockSettingDataResponse(
        const OplusRadioResponseInfo& in_info, int32_t in_result) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::setSimlockOfflineLockResponse(
        const OplusRadioResponseInfo& in_info) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus OplusRadioResponse::getSimlockOfflineLockResponse(
        const OplusRadioResponseInfo& in_info, const std::vector<int32_t>& in_retryCount) {
    return ndk::ScopedAStatus::ok();
}

};  // namespace aidl::vendor::oplus::hardware::radio
