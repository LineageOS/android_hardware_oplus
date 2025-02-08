/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ConsumerIr.h"

#include <aidl/android/hardware/ir/ConsumerIrFreqRange.h>
#include <android-base/logging.h>
#include <android-base/unique_fd.h>
#include <oplus/oplus_ir_core.h>

namespace aidl {
namespace android {
namespace hardware {
namespace ir {

static constexpr int32_t MAX_PATTERN_TIME = 2000000;
static constexpr int32_t MIN_FREQ = 20000;
static constexpr int32_t MAX_FREQ = 60000;

ConsumerIr::ConsumerIr() : supportedFreqs({{MIN_FREQ, MAX_FREQ}}) {}

::ndk::ScopedAStatus ConsumerIr::getCarrierFreqs(std::vector<ConsumerIrFreqRange>* _aidl_return) {
    *_aidl_return = supportedFreqs;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus ConsumerIr::transmit(int32_t in_carrierFreqHz,
                                          const std::vector<int32_t>& in_pattern) {
    if (in_carrierFreqHz < MIN_FREQ || in_carrierFreqHz > MAX_FREQ) {
        LOG(ERROR) << "Invalid carrier frequency: " << in_carrierFreqHz;
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    int32_t totalTime = 0;
    for (int32_t value : in_pattern) {
        if (value < 0) {
            LOG(ERROR) << "Invalid pattern value: " << value;
            return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
        }
        totalTime += value;
    }

    if (totalTime > MAX_PATTERN_TIME) {
        LOG(ERROR) << "Pattern is too long: " << totalTime << " us";
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    ::android::base::unique_fd fd(open("/dev/oplus_consumer_ir", O_WRONLY));
    if (!fd.ok()) {
        LOG(ERROR) << "Failed to open /dev/oplus_consumer_ir: " << strerror(errno);
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    size_t paramsSize = sizeof(struct pattern_params) + in_pattern.size() * sizeof(int32_t);
    auto params = std::unique_ptr<struct pattern_params, decltype(&free)>(
            static_cast<pattern_params*>(malloc(paramsSize)), free);
    if (!params) {
        LOG(ERROR) << "Failed to allocate memory for IR params";
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    params->carrier_freq = in_carrierFreqHz;
    params->size = in_pattern.size();
    memcpy(params->pattern, in_pattern.data(), in_pattern.size() * sizeof(int32_t));

    int result = ioctl(fd, IR_SEND_PATTERN, params.get());

    return result < 0 ? ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION)
                      : ::ndk::ScopedAStatus::ok();
}

}  // namespace ir
}  // namespace hardware
}  // namespace android
}  // namespace aidl
