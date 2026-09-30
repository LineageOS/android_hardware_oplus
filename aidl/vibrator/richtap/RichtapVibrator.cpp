/*
 * Copyright (C) 2019 The Android Open Source Project
 * Copyright (C) 2026 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "RichtapVibrator.h"

#include <android-base/logging.h>
#include <fcntl.h>
#include <inttypes.h>
#include <log/log.h>
#include <string.h>
#include <unistd.h>

#include <memory>
#include <thread>

namespace aidl::vendor::aac::hardware::richtap::vibrator {

void RichtapVibrator::send_handle_result(const std::shared_ptr<IRichtapCallback>& callback,
                                         int32_t timeout, int32_t result) {
    if (callback != nullptr) {
        std::thread([=] {
            usleep(timeout * 1000);
            callback->onCallback(result);
        }).detach();
    }
}

ndk::ScopedAStatus RichtapVibrator::init(const std::shared_ptr<IRichtapCallback>& callback) {
    // The AAC library is initialized by the Vibrator service constructor
    // (aac_vibra_init() + aac_vibra_looper_start()), which the driver only accepts once, so the
    // extension just records that RichTap is usable from here on.
    m_richtap_support = true;
    send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_SUCCESS);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RichtapVibrator::off(const std::shared_ptr<IRichtapCallback>& callback) {
    if (!m_richtap_support) {
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_NOT_SUPPORT);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }
    if (aac_vibra_off() != 0) {
        ALOGE("aac_vibra_off failed");
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_FAILED);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_STATE));
    }
    send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_SUCCESS);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RichtapVibrator::on(int32_t timeoutMs,
                                       const std::shared_ptr<IRichtapCallback>& callback) {
    if (!m_richtap_support) {
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_NOT_SUPPORT);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }
    int32_t timeout_ms = aac_vibra_looper_on(timeoutMs);
    if (timeout_ms < 0) {
        ALOGE("aac_vibra_looper_on failed: %d", timeout_ms);
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_FAILED);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_STATE));
    }
    send_handle_result(callback, timeout_ms, RICHTAP_HANDLE_SUCCESS);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RichtapVibrator::setAmplitude(
        int32_t amplitude, const std::shared_ptr<IRichtapCallback>& callback) {
    if (!m_richtap_support) {
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_NOT_SUPPORT);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }
    if (aac_vibra_setAmplitude(static_cast<uint8_t>(amplitude)) != 0) {
        ALOGE("aac_vibra_setAmplitude failed");
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_FAILED);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_STATE));
    }
    send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_SUCCESS);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RichtapVibrator::perform(int32_t effect, int8_t strength,
                                            const std::shared_ptr<IRichtapCallback>& callback,
                                            int32_t* _aidl_return) {
    if (!m_richtap_support) {
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_NOT_SUPPORT);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }
    long playLengthMs = aac_vibra_looper_prebaked_effect(static_cast<uint32_t>(effect),
                                                         static_cast<int32_t>(strength));
    if (playLengthMs < 0) {
        ALOGE("aac_vibra_looper_prebaked_effect failed: %ld", playLengthMs);
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_FAILED);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_SERVICE_SPECIFIC));
    }
    *_aidl_return = playLengthMs;
    send_handle_result(callback, playLengthMs, RICHTAP_HANDLE_SUCCESS);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RichtapVibrator::performEnvelope(
        const std::vector<int32_t>& envInfo, bool fastFlag,
        const std::shared_ptr<IRichtapCallback>& callback) {
    if (!m_richtap_support) {
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_NOT_SUPPORT);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }
    if (envInfo.empty()) {
        ALOGE("performEnvelope: empty envInfo");
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_FAILED);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_STATE));
    }
    int32_t timeout_ms = aac_vibra_looper_envelope(envInfo.data(),
                                                   static_cast<uint32_t>(envInfo.size()), fastFlag);
    if (timeout_ms < 0) {
        ALOGE("aac_vibra_looper_envelope failed: %d", timeout_ms);
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_FAILED);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_STATE));
    }
    send_handle_result(callback, timeout_ms, RICHTAP_HANDLE_SUCCESS);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RichtapVibrator::performRtp(const ndk::ScopedFileDescriptor& pfd,
                                               const std::shared_ptr<IRichtapCallback>& callback) {
    if (!m_richtap_support) {
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_NOT_SUPPORT);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }
    if (pfd.get() < 0) {
        ALOGE("performRtp: no fd");
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_FAILED);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_STATE));
    }
    int fd = fcntl(pfd.get(), F_DUPFD, 0);
    if (fd < 0) {
        ALOGE("performRtp: dup of fd %d failed", pfd.get());
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_FAILED);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_STATE));
    }

    int32_t timeout_ms = aac_vibra_looper_rtp(fd);
    bool success = (timeout_ms > 0);

    // aac_vibra_looper_rtp() queues a work item that reads from the fd asynchronously, so the
    // duplicate must stay open until the looper is done with it. Keep it alive for the effect
    // duration (or a short grace period on error) from a detached thread.
    auto fdGuard = std::shared_ptr<int>(new int(fd), [](int* p) {
        close(*p);
        delete p;
    });
    int graceMs = success ? (timeout_ms + 1000) : 1000;
    if (callback != nullptr) {
        std::thread([callback, success, fdGuard, graceMs]() {
            usleep(graceMs * 1000);
            callback->onCallback(success ? RICHTAP_HANDLE_SUCCESS : RICHTAP_HANDLE_FAILED);
        }).detach();
    } else {
        std::thread([fdGuard, graceMs]() { usleep(graceMs * 1000); }).detach();
    }

    if (!success) {
        ALOGE("aac_vibra_looper_rtp failed: %d", timeout_ms);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_STATE));
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RichtapVibrator::performHe(int32_t looper, int32_t interval, int32_t amplitude,
                                              int32_t freq, const std::vector<int32_t>& he,
                                              const std::shared_ptr<IRichtapCallback>& callback) {
    if (!m_richtap_support) {
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_NOT_SUPPORT);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }
    if (he.empty()) {
        ALOGE("performHe: empty data");
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_FAILED);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_STATE));
    }
    int32_t timeout_ms = aac_vibra_looper_post(he.data(), static_cast<int32_t>(he.size()), interval,
                                               looper, amplitude, freq);
    if (timeout_ms < 0) {
        ALOGE("aac_vibra_looper_post failed: %d", timeout_ms);
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_FAILED);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_STATE));
    }
    send_handle_result(callback, timeout_ms, RICHTAP_HANDLE_SUCCESS);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RichtapVibrator::performHeParam(
        int32_t interval, int32_t amplitude, int32_t freq,
        const std::shared_ptr<IRichtapCallback>& callback) {
    if (!m_richtap_support) {
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_NOT_SUPPORT);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }
    bool ret;
    if (interval == 0 && amplitude == 0 && freq == 0) {
        ret = aac_vibra_looper_stopPerformHe();
    } else {
        ret = aac_vibra_looper_performParam(interval, amplitude, freq);
    }
    if (!ret) {
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_FAILED);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_STATE));
    }
    send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_SUCCESS);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RichtapVibrator::setHapticParam(
        const std::vector<int32_t>& data, const int32_t length,
        const std::shared_ptr<IRichtapCallback>& callback) {
    if (length == 2 && data[0] == HAPTIC_PARAM_DRC_MARK) {
        if (data[1] >= 0 && data[1] <= HAPTIC_PARAM_MAX_DRC) {
            aac_vibra_set_drc_mode(data[1]);
        }
        ALOGD("setHapticParam DRC mode: %d", data[1]);
    } else if (!data.empty()) {
        ALOGD("setHapticParam: %zu parameters", data.size());
        aac_vibra_update_parameter(data.data(), length);
    }
    send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_SUCCESS);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RichtapVibrator::setDynamicScale(
        int32_t scale, const std::shared_ptr<IRichtapCallback>& callback) {
    if (!m_richtap_support) {
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_NOT_SUPPORT);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }
    if (aac_vibra_dynamic_scale(static_cast<uint8_t>(scale)) != 0) {
        ALOGE("aac_vibra_dynamic_scale failed");
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_FAILED);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_STATE));
    }
    send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_SUCCESS);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RichtapVibrator::setF0(int32_t f0,
                                          const std::shared_ptr<IRichtapCallback>& callback) {
    if (!m_richtap_support) {
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_NOT_SUPPORT);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }
    if (aac_vibra_setting_f0(f0) != 0) {
        ALOGE("aac_vibra_setting_f0 failed");
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_FAILED);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_STATE));
    }
    send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_SUCCESS);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus RichtapVibrator::stop(const std::shared_ptr<IRichtapCallback>& callback) {
    if (!m_richtap_support) {
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_NOT_SUPPORT);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }
    if (!aac_vibra_looper_stopPerformHe()) {
        send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_FAILED);
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_STATE));
    }
    send_handle_result(callback, DEFAULT_RETURN_TIME_OUT, RICHTAP_HANDLE_SUCCESS);
    return ndk::ScopedAStatus::ok();
}

}  // namespace aidl::vendor::aac::hardware::richtap::vibrator
