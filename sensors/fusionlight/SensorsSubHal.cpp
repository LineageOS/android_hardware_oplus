/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "SensorsSubHal.h"

#include <dlfcn.h>

#include <algorithm>
#include <utility>

#include <android-base/logging.h>
#include <hardware/sensors.h>

using ::android::hardware::sensors::V1_0::MetaDataEventType;
using ::android::hardware::sensors::V2_0::implementation::ScopedWakelock;
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

constexpr auto kTypeUnderScreenRgbSensor = static_cast<SensorType>(33171070);
constexpr auto kTypeMainFusionConfigSensor = static_cast<SensorType>(33171042);
constexpr int32_t kVirtualSensorHandleBase = 0x10000;

bool IsVirtualHandle(int32_t sensor_handle) {
    return sensor_handle >= kVirtualSensorHandleBase;
}

int32_t ToVirtualHandle(int32_t sensor_handle) {
    return IsVirtualHandle(sensor_handle) ? sensor_handle
                                          : sensor_handle + kVirtualSensorHandleBase;
}

};  // anonymous namespace

SensorsSubHal::SensorsSubHal()
    : lib_handle_(dlopen(kLibName, RTLD_NOW),
                  [](void* handle) {
                      if (handle != nullptr) {
                          dlclose(handle);
                      }
                  }),
      fusion_light_([this](Event event) { emitFusionEvent(std::move(event)); }) {
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
    const int32_t fusion_light_handle = fusion_light_handle_;
    const int32_t physical_handle = high_pwm_handle_;
    const bool fusion_light_request = sensor_handle == fusion_light_handle;

    if (sensor_handle != physical_handle && !fusion_light_request) {
        return impl_->activate(sensor_handle, enabled);
    }

    {
        std::lock_guard lock(control_mutex_);

        const bool fusion_light_enabled = fusion_light_enabled_.load();
        const bool currently_enabled = fusion_light_request ? fusion_light_enabled : raw_enabled_;
        if (currently_enabled == enabled) {
            return Result::OK;
        }

        const bool was_physically_enabled = raw_enabled_ || fusion_light_enabled;
        const bool should_be_physically_enabled =
                fusion_light_request ? raw_enabled_ || enabled : enabled || fusion_light_enabled;
        if (was_physically_enabled != should_be_physically_enabled) {
            auto result = impl_->activate(physical_handle, should_be_physically_enabled);
            if (result != Result::OK) {
                return result;
            }
        }

        if (fusion_light_request) {
            fusion_light_enabled_.store(enabled);
        } else {
            raw_enabled_ = enabled;
        }
    }

    if (fusion_light_request) {
        fusion_light_.setEnabled(enabled);
    }
    return Result::OK;
}

Return<Result> SensorsSubHal::batch(int32_t sensor_handle, int64_t sampling_period_ns,
                                    int64_t max_report_latency_ns) {
    const int32_t physical_handle =
            sensor_handle == fusion_light_handle_ ? high_pwm_handle_ : sensor_handle;
    return impl_->batch(physical_handle, sampling_period_ns, max_report_latency_ns);
}

Return<Result> SensorsSubHal::flush(int32_t sensor_handle) {
    const int32_t fusion_light_handle = fusion_light_handle_;
    const int32_t high_pwm_handle = high_pwm_handle_;
    const bool fusion_light_request = sensor_handle == fusion_light_handle;

    if (sensor_handle != high_pwm_handle && !fusion_light_request) {
        return impl_->flush(sensor_handle);
    }

    std::lock_guard flush_call_lock(flush_call_mutex_);
    {
        std::lock_guard lock(control_mutex_);
        const bool enabled = fusion_light_request ? fusion_light_enabled_.load() : raw_enabled_;
        if (!enabled) {
            return Result::BAD_VALUE;
        }
    }

    const FlushTarget target = fusion_light_request ? FlushTarget::kFusionLight : FlushTarget::kRaw;
    {
        std::lock_guard lock(flush_mutex_);
        pending_flushes_.push_back(target);
    }

    auto result = impl_->flush(high_pwm_handle);
    if (result != Result::OK) {
        std::lock_guard lock(flush_mutex_);
        if (!pending_flushes_.empty()) {
            pending_flushes_.pop_back();
        }
    }
    return result;
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
    if (sensor_handle == fusion_light_handle_) {
        callback(Result::BAD_VALUE, 0);
        return Void();
    }
    return impl_->configDirectReport(sensor_handle, channel_handle, rate, callback);
}

Return<void> SensorsSubHal::getSensorsList_2_1(ISensors::getSensorsList_2_1_cb callback) {
    return impl_->getSensorsList_2_1([&](const auto& source) {
        const auto high_pwm = std::find_if(source.begin(), source.end(), [](const auto& sensor) {
            return sensor.type == kTypeUnderScreenRgbSensor;
        });

        if (high_pwm == source.end()) {
            callback(source);
            return;
        }

        auto sensors = hidl_vec<SensorInfo>(source.size() + 1);
        std::copy(source.begin(), source.end(), sensors.begin());
        sensors[source.size()] = *high_pwm;
        const int32_t high_pwm_handle = high_pwm->sensorHandle;
        const int32_t fusion_light_handle = ToVirtualHandle(high_pwm_handle);
        sensors[source.size()].sensorHandle = fusion_light_handle;
        sensors[source.size()].name = "Oplus FusionLight Sensor";
        sensors[source.size()].type = SensorType::LIGHT;
        sensors[source.size()].typeAsString = "";

        bool initialize_fusion_light = false;
        if (high_pwm_handle_ < 0 && fusion_light_handle_ < 0) {
            high_pwm_handle_ = high_pwm_handle;
            fusion_light_handle_ = fusion_light_handle;
            initialize_fusion_light = true;
        } else if (high_pwm_handle_ != high_pwm_handle ||
                   fusion_light_handle_ != fusion_light_handle) {
            LOG(ERROR) << "FusionLight sensor handles changed after initial enumeration: "
                       << high_pwm_handle_ << "/" << fusion_light_handle_ << " -> "
                       << high_pwm_handle << "/" << fusion_light_handle;
        }
        if (initialize_fusion_light) {
            const auto main_profile_sensor = std::find_if(
                    source.begin(), source.end(),
                    [](const auto& sensor) { return sensor.type == kTypeMainFusionConfigSensor; });
            const auto profile_sensor =
                    main_profile_sensor == source.end() ? high_pwm : main_profile_sensor;
            fusion_light_.initialize(profile_sensor->name.c_str());
            LOG(INFO) << "Added FusionLight sensor " << fusion_light_handle
                      << " backed by high-PWM RGB sensor " << high_pwm_handle;
        }
        callback(sensors);
    });
}

Return<Result> SensorsSubHal::injectSensorData_2_1(const Event& event) {
    if (event.sensorHandle == fusion_light_handle_) {
        return Result::BAD_VALUE;
    }
    return impl_->injectSensorData_2_1(event);
}

Return<void> SensorsSubHal::debug(const hidl_handle& fd, const hidl_vec<hidl_string>& args) {
    return impl_->debug(fd, args);
}

const std::string SensorsSubHal::getName() {
    return impl_->getName();
}

Return<Result> SensorsSubHal::initialize(const sp<IHalProxyCallback>& hal_proxy_callback) {
    hal_proxy_callback_ = hal_proxy_callback;
    return impl_->initialize(this);
}

Return<void> SensorsSubHal::onDynamicSensorsConnected(
        const hidl_vec<V1_0::SensorInfo>& sensor_infos) {
    return hal_proxy_callback_->onDynamicSensorsConnected(sensor_infos);
}

Return<void> SensorsSubHal::onDynamicSensorsDisconnected(const hidl_vec<int32_t>& sensor_handles) {
    return hal_proxy_callback_->onDynamicSensorsDisconnected(sensor_handles);
}

Return<void> SensorsSubHal::onDynamicSensorsConnected_2_1(
        const hidl_vec<V2_1::SensorInfo>& sensor_infos) {
    return hal_proxy_callback_->onDynamicSensorsConnected_2_1(sensor_infos);
}

void SensorsSubHal::postEvents(const std::vector<Event>& events, ScopedWakelock wakelock) {
    std::vector<Event> output;
    output.reserve(events.size());
    const int32_t physical_handle = high_pwm_handle_;
    const int32_t fusion_light_handle = fusion_light_handle_;

    for (const auto& event : events) {
        if (event.sensorType == SensorType::META_DATA &&
            event.u.meta.what == MetaDataEventType::META_DATA_FLUSH_COMPLETE &&
            event.sensorHandle == physical_handle) {
            FlushTarget flush_target = FlushTarget::kRaw;
            bool requested = false;
            {
                std::lock_guard lock(flush_mutex_);
                if (!pending_flushes_.empty()) {
                    flush_target = pending_flushes_.front();
                    pending_flushes_.pop_front();
                    requested = true;
                }
            }
            Event flush_event = event;
            if (requested && flush_target == FlushTarget::kFusionLight) {
                flush_event.sensorHandle = fusion_light_handle;
                fusion_light_.completeFlush(std::move(flush_event));
                continue;
            }
            output.push_back(std::move(flush_event));
            continue;
        }

        // Forward every non-flush event unchanged.
        output.push_back(event);
        if (event.sensorType != kTypeUnderScreenRgbSensor) {
            continue;
        }

        if (fusion_light_enabled_.load() && fusion_light_handle >= 0) {
            fusion_light_.process(event, fusion_light_handle);
        }
    }

    if (!output.empty()) {
        hal_proxy_callback_->postEvents(output, std::move(wakelock));
    }
}

void SensorsSubHal::emitFusionEvent(Event event) {
    if (!fusion_light_enabled_.load() || hal_proxy_callback_ == nullptr) {
        return;
    }
    std::vector<Event> events;
    events.push_back(std::move(event));
    hal_proxy_callback_->postEvents(events, hal_proxy_callback_->createScopedWakelock(false));
}

ScopedWakelock SensorsSubHal::createScopedWakelock(bool lock) {
    return hal_proxy_callback_->createScopedWakelock(lock);
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
