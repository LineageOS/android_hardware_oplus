/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "SensorsSubHal.h"

#include <dlfcn.h>

#include <android-base/logging.h>

using ::android::hardware::sensors::V2_1::implementation::ISensorsSubHal;

namespace android {
namespace hardware {
namespace sensors {
namespace V2_1 {
namespace subhal {
namespace implementation {
namespace fusionlight {
namespace {

#ifdef BACKEND_SUBHAL_LIB_NAME
constexpr auto kLibName = BACKEND_SUBHAL_LIB_NAME;
#else
constexpr auto kLibName = "sensors.qsh.so";
#endif
};  // anonymous namespace

SensorsSubHal::SensorsSubHal()
    : lib_handle_(dlopen(kLibName, RTLD_NOW), [](void* handle) {
          if (handle != nullptr) {
              dlclose(handle);
          }
      }) {
    if (!lib_handle_) {
        LOG(FATAL) << "dlopen " << kLibName << " failed: " << dlerror();
    }

    const auto get_sub_hal = reinterpret_cast<ISensorsSubHal* (*)(uint32_t*)>(
            dlsym(lib_handle_.get(), "sensorsHalGetSubHal_2_1"));
    if (get_sub_hal == nullptr) {
        LOG(FATAL) << "sensorsHalGetSubHal_2_1 is missing from " << kLibName;
    }
    uint32_t version;
    impl_ = get_sub_hal(&version);
    if (impl_ == nullptr) {
        LOG(FATAL) << "sensorsHalGetSubHal_2_1 returned null";
    }
}

Return<Result> SensorsSubHal::setOperationMode(OperationMode mode) {
    return impl_->setOperationMode(mode);
}

Return<Result> SensorsSubHal::activate(int32_t sensor_handle, bool enabled) {
    return impl_->activate(sensor_handle, enabled);
}

Return<Result> SensorsSubHal::batch(int32_t sensor_handle, int64_t sampling_period_ns,
                                    int64_t max_report_latency_ns) {
    return impl_->batch(sensor_handle, sampling_period_ns, max_report_latency_ns);
}

Return<Result> SensorsSubHal::flush(int32_t sensor_handle) {
    return impl_->flush(sensor_handle);
}

Return<void> SensorsSubHal::registerDirectChannel(const SharedMemInfo& mem,
                                                  ISensors::registerDirectChannel_cb callback) {
    return impl_->registerDirectChannel(mem, callback);
}

Return<Result> SensorsSubHal::unregisterDirectChannel(int32_t channel_handle) {
    return impl_->unregisterDirectChannel(channel_handle);
}

Return<void> SensorsSubHal::configDirectReport(int32_t sensor_handle, int32_t channel_handle,
                                               RateLevel rate,
                                               ISensors::configDirectReport_cb callback) {
    return impl_->configDirectReport(sensor_handle, channel_handle, rate, callback);
}

Return<void> SensorsSubHal::getSensorsList_2_1(ISensors::getSensorsList_2_1_cb callback) {
    return impl_->getSensorsList_2_1(callback);
}

Return<Result> SensorsSubHal::injectSensorData_2_1(const Event& event) {
    return impl_->injectSensorData_2_1(event);
}

Return<void> SensorsSubHal::debug(const hidl_handle& fd, const hidl_vec<hidl_string>& args) {
    return impl_->debug(fd, args);
}

const std::string SensorsSubHal::getName() {
    return impl_->getName();
}

Return<Result> SensorsSubHal::initialize(const sp<IHalProxyCallback>& hal_proxy_callback) {
    return impl_->initialize(hal_proxy_callback);
}

}  // namespace fusionlight
}  // namespace implementation
}  // namespace subhal
}  // namespace V2_1
}  // namespace sensors
}  // namespace hardware
}  // namespace android

ISensorsSubHal* sensorsHalGetSubHal_2_1(uint32_t* version) {
    static ::android::hardware::sensors::V2_1::subhal::implementation::fusionlight::SensorsSubHal
            sub_hal;
    *version = SUB_HAL_2_1_VERSION;
    return &sub_hal;
}
