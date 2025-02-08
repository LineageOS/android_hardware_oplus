/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <V2_1/SubHal.h>

namespace android {
namespace hardware {
namespace sensors {
namespace V2_1 {
namespace subhal {
namespace implementation {
namespace wrapper {

using ::android::hardware::sensors::V1_0::OperationMode;
using ::android::hardware::sensors::V1_0::RateLevel;
using ::android::hardware::sensors::V1_0::Result;
using ::android::hardware::sensors::V1_0::SharedMemInfo;
using ::android::hardware::sensors::V2_1::Event;
using ::android::hardware::sensors::V2_1::implementation::IHalProxyCallback;
using ::android::hardware::sensors::V2_1::implementation::ISensorsSubHal;

class SensorsSubHal : public ISensorsSubHal, public IHalProxyCallback {
  public:
    SensorsSubHal();

    // ISensors
    Return<Result> setOperationMode(OperationMode mode) override;
    Return<Result> activate(int32_t sensor_handle, bool enabled) override;
    Return<Result> batch(int32_t sensor_handle, int64_t sampling_period_ns,
                         int64_t max_report_latency_ns) override;
    Return<Result> flush(int32_t sensor_handle) override;
    Return<void> registerDirectChannel(const SharedMemInfo& mem,
                                       ISensors::registerDirectChannel_cb _hidl_cb) override;
    Return<Result> unregisterDirectChannel(int32_t channel_handle) override;
    Return<void> configDirectReport(int32_t sensor_handle, int32_t channel_handle, RateLevel rate,
                                    ISensors::configDirectReport_cb _hidl_cb) override;
    Return<void> getSensorsList_2_1(ISensors::getSensorsList_2_1_cb _hidl_cb) override;
    Return<Result> injectSensorData_2_1(const Event& event) override;

    // ISensorsSubHal
    Return<void> debug(const hidl_handle& fd, const hidl_vec<hidl_string>& args) override;
    const std::string getName() override;
    Return<Result> initialize(const sp<IHalProxyCallback>& hal_proxy_callback) override;

    // ISensorsCallback
    Return<void> onDynamicSensorsConnected(const hidl_vec<V1_0::SensorInfo>& sensor_infos) override;
    Return<void> onDynamicSensorsDisconnected(const hidl_vec<int32_t>& sensor_handles) override;
    Return<void> onDynamicSensorsConnected_2_1(
            const hidl_vec<V2_1::SensorInfo>& sensor_infos) override;

    // IHalProxyCallback
    void postEvents(const std::vector<Event>& events, ScopedWakelock wakelock) override;
    ScopedWakelock createScopedWakelock(bool lock) override;

  private:
    std::unique_ptr<void, std::function<void(void*)>> lib_handle_;
    V2_1::implementation::ISensorsSubHal* impl_;
    sp<IHalProxyCallback> hal_proxy_callback_;

    std::unordered_map<int32_t, SensorType> handle_type_;
};

}  // namespace wrapper
}  // namespace implementation
}  // namespace subhal
}  // namespace V2_1
}  // namespace sensors
}  // namespace hardware
}  // namespace android
