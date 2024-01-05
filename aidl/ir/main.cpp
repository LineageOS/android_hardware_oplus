/*
 * Copyright (C) 2021 The Android Open Source Project
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

#include <aidl/android/hardware/ir/BnConsumerIr.h>
#include <aidl/android/hardware/ir/ConsumerIrFreqRange.h>
#include <android-base/logging.h>
#include <android-base/unique_fd.h>
#include <android/binder_interface_utils.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <numeric>

#include <oplus/oplus_ir_core.h>

using ::aidl::android::hardware::ir::ConsumerIrFreqRange;

namespace aidl::android::hardware::ir {

static constexpr int32_t MAX_PATTERN_TIME = 2000000;

static const std::vector<ConsumerIrFreqRange> kSupportedFreqs = {
    {30000, 30000},
    {33000, 33000},
    {36000, 36000},
    {38000, 38000},
    {40000, 40000},
    {56000, 56000},
    {60000, 60000},
};

static bool isSupportedFreq(int32_t freq) {
    for (const auto& range : kSupportedFreqs) {
        if (freq >= range.minHz && freq <= range.maxHz) return true;
    }
    return false;
}

class ConsumerIr : public BnConsumerIr {
  public:
    ConsumerIr() = default;
  private:
    ::ndk::ScopedAStatus getCarrierFreqs(std::vector<ConsumerIrFreqRange>* _aidl_return) override;
    ::ndk::ScopedAStatus transmit(int32_t in_carrierFreqHz,
                                  const std::vector<int32_t>& in_pattern) override;
};

::ndk::ScopedAStatus ConsumerIr::getCarrierFreqs(std::vector<ConsumerIrFreqRange>* _aidl_return) {
    *_aidl_return = kSupportedFreqs;
    return ::ndk::ScopedAStatus::ok();
}

::ndk::ScopedAStatus ConsumerIr::transmit(int32_t in_carrierFreqHz,
                                          const std::vector<int32_t>& in_pattern) {
    if (!isSupportedFreq(in_carrierFreqHz)) {
        LOG(ERROR) << "Unsupported carrier frequency: " << in_carrierFreqHz << " Hz";
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

    ::android::base::unique_fd oplusConsumerIrFd(open("/dev/oplus_consumer_ir", O_WRONLY));
    if (!oplusConsumerIrFd.ok()) {
        LOG(ERROR) << "Failed to open /dev/oplus_consumer_ir: " << strerror(errno);
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    size_t paramsSize = sizeof(struct pattern_params) + in_pattern.size() * sizeof(int32_t);
    auto params = std::unique_ptr<struct pattern_params, decltype(&free)>(
        static_cast<pattern_params*>(malloc(paramsSize)), free
    );
    if (!params) {
        LOG(ERROR) << "Failed to allocate memory for IR params";
        return ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    params->carrier_freq = in_carrierFreqHz;
    params->size = in_pattern.size();
    memcpy(params->pattern, in_pattern.data(), in_pattern.size() * sizeof(int32_t));

    int result = ioctl(oplusConsumerIrFd, IR_SEND_PATTERN, params.get());

    return result < 0 ?
        ::ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION) :
        ::ndk::ScopedAStatus::ok();
}

}  // namespace aidl::android::hardware::ir

using aidl::android::hardware::ir::ConsumerIr;

int main() {
    auto binder = ::ndk::SharedRefBase::make<ConsumerIr>();
    const std::string name = std::string() + ConsumerIr::descriptor + "/default";
    CHECK_EQ(STATUS_OK, AServiceManager_addService(binder->asBinder().get(), name.c_str()))
            << "Failed to register " << name;

    ABinderProcess_setThreadPoolMaxThreadCount(0);
    ABinderProcess_joinThreadPool();

    return EXIT_FAILURE;  // should not reached
}
