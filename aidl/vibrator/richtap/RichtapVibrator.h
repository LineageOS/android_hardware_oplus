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

#pragma once

#include <aidl/vendor/aac/hardware/richtap/vibrator/BnRichtapCallback.h>
#include <aidl/vendor/aac/hardware/richtap/vibrator/BnRichtapVibrator.h>

#include "aac_vibra_function.h"

#define DEFAULT_RETURN_TIME_OUT 10

namespace aidl::vendor::aac::hardware::richtap::vibrator {

using aidl::vendor::aac::hardware::richtap::vibrator::IRichtapCallback;

enum RICHTAP_HANDLE_RESULT {
    RICHTAP_HANDLE_SUCCESS = 1,
    RICHTAP_HANDLE_FAILED,
    RICHTAP_HANDLE_NOT_SUPPORT,
    RICHTAP_HANDLE_INVALID,
};

/**
 * RichTap extension of the OPLUS vibrator HAL.
 *
 * This is not a standalone service: it is attached to the standard
 * android.hardware.vibrator.IVibrator/default service as an AIBinder extension, and is reached by
 * the framework through IBinder.getExtension(). It only implements the RichTap-specific vendor
 * AIDL calls; standard vibration is handled by the Vibrator class.
 */
class RichtapVibrator : public BnRichtapVibrator {
  private:
    bool m_richtap_support = false;
    void send_handle_result(const std::shared_ptr<IRichtapCallback>& callback, int32_t timeout,
                            int32_t result);

  public:
    ndk::ScopedAStatus init(const std::shared_ptr<IRichtapCallback>& in_callback) override;
    ndk::ScopedAStatus setDynamicScale(
            int32_t in_scale, const std::shared_ptr<IRichtapCallback>& in_callback) override;
    ndk::ScopedAStatus setF0(int32_t in_f0,
                             const std::shared_ptr<IRichtapCallback>& in_callback) override;
    ndk::ScopedAStatus stop(const std::shared_ptr<IRichtapCallback>& in_callback) override;
    ndk::ScopedAStatus setAmplitude(
            int32_t in_amplitude, const std::shared_ptr<IRichtapCallback>& in_callback) override;
    ndk::ScopedAStatus performHeParam(int32_t in_interval, int32_t in_amplitude, int32_t in_freq,
                                      const std::shared_ptr<IRichtapCallback>& in_callback) override;
    ndk::ScopedAStatus off(const std::shared_ptr<IRichtapCallback>& in_callback) override;
    ndk::ScopedAStatus on(int32_t in_timeoutMs,
                          const std::shared_ptr<IRichtapCallback>& in_callback) override;
    ndk::ScopedAStatus perform(int32_t in_effect_id, int8_t in_strength,
                               const std::shared_ptr<IRichtapCallback>& in_callback,
                               int32_t* _aidl_return) override;
    ndk::ScopedAStatus performEnvelope(
            const std::vector<int32_t>& in_envInfo, bool in_fastFlag,
            const std::shared_ptr<IRichtapCallback>& in_callback) override;
    ndk::ScopedAStatus performRtp(const ndk::ScopedFileDescriptor& in_hdl,
                                  const std::shared_ptr<IRichtapCallback>& in_callback) override;
    ndk::ScopedAStatus performHe(int32_t in_looper, int32_t in_interval, int32_t in_amplitude,
                                 int32_t in_freq, const std::vector<int32_t>& in_he,
                                 const std::shared_ptr<IRichtapCallback>& in_callback) override;
    ndk::ScopedAStatus setHapticParam(
            const std::vector<int32_t>& data, const int32_t length,
            const std::shared_ptr<IRichtapCallback>& in_callback) override;
};

}  // namespace aidl::vendor::aac::hardware::richtap::vibrator
