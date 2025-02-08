/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "FusionLightUtils.h"

#include <aidl/vendor/lineage/oplus_fusionlight/IScreenSampler.h>
#include <android-base/macros.h>
#include <android/hardware/sensors/2.1/types.h>

namespace android {
namespace hardware {
namespace sensors {
namespace V2_1 {
namespace subhal {
namespace implementation {
namespace fusionlight {

using ::android::hardware::sensors::V2_1::Event;

class FusionLightProcessor final {
  public:
    using EmitCallback = std::function<void(Event)>;

    explicit FusionLightProcessor(EmitCallback emit_callback);
    ~FusionLightProcessor();

    DISALLOW_COPY_AND_ASSIGN(FusionLightProcessor);

    bool initialize(const std::string& profile_sensor_name);
    void setEnabled(bool enabled);

    void process(const Event& event, int32_t fusion_light_handle);
    void completeFlush(Event event);

  private:
    struct SharedState;
    class SampleCallback;

    bool initializeLocked();
    bool ensureSession();
    void enqueueReady(Event event);
    void processScreenOff(Event event, const Channels& raw_channels, int32_t brightness);

    std::shared_ptr<SharedState> state_;
    std::mutex lifecycle_mutex_;
    std::shared_ptr<aidl::vendor::lineage::oplus_fusionlight::IScreenSampler> sampler_;
    std::shared_ptr<SampleCallback> callback_;
    std::chrono::steady_clock::time_point next_service_attempt_{};
    std::string profile_sensor_name_;
    int64_t session_ = -1;
};

}  // namespace fusionlight
}  // namespace implementation
}  // namespace subhal
}  // namespace V2_1
}  // namespace sensors
}  // namespace hardware
}  // namespace android
