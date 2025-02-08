/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "CwbSampler.h"

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

    bool initializeLocked();
    void enqueueReady(Event event, const char* reason);
    void processScreenOff(Event event, const Channels& raw_channels, int32_t brightness);

    std::shared_ptr<SharedState> state_;
    CwbSampler sampler_;
    std::mutex lifecycle_mutex_;
    std::string profile_sensor_name_;
};

}  // namespace fusionlight
}  // namespace implementation
}  // namespace subhal
}  // namespace V2_1
}  // namespace sensors
}  // namespace hardware
}  // namespace android
