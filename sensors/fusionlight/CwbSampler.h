/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "FusionLightUtils.h"

#include <android-base/macros.h>

#include <condition_variable>
#include <functional>
#include <memory>
#include <optional>
#include <thread>

namespace android {
namespace hardware {
namespace sensors {
namespace V2_1 {
namespace subhal {
namespace implementation {
namespace fusionlight {

struct CwbSample {
    int32_t r = 0;
    int32_t g = 0;
    int32_t b = 0;
    int64_t frame_start_ns = 0;
    int64_t frame_end_ns = 0;
};

class CwbSampler final {
  public:
    using SampleCallback = std::function<void(std::optional<CwbSample>)>;

    explicit CwbSampler(SampleCallback sample_callback);
    ~CwbSampler();

    DISALLOW_COPY_AND_ASSIGN(CwbSampler);

    void start(const FusionConfig& config);
    void stop();
    void requestSample();

  private:
    struct SharedState;
    class DisplayCallback;

    static void threadLoop(const std::shared_ptr<SharedState>& state);

    std::shared_ptr<SharedState> state_;
    std::thread thread_;
};

}  // namespace fusionlight
}  // namespace implementation
}  // namespace subhal
}  // namespace V2_1
}  // namespace sensors
}  // namespace hardware
}  // namespace android
