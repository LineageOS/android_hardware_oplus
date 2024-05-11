/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include "HalProxyCallback.h"
#include "V2_0/SubHal.h"
#include "V2_1/SubHal.h"

#include "android/hardware/sensors/1.0/ISensors.h"
#include "android/hardware/sensors/1.0/types.h"
#include "android/hardware/sensors/2.0/ISensors.h"
#include "android/hardware/sensors/2.0/ISensorsCallback.h"
#include "android/hardware/sensors/2.1/ISensors.h"
#include "android/hardware/sensors/2.1/ISensorsCallback.h"
#include "android/hardware/sensors/2.1/types.h"

#include <utils/LightRefBase.h>

#include <cassert>

namespace android {
namespace hardware {
namespace sensors {
namespace V2_1 {
namespace implementation {

/**
 * The following subHal wrapper classes abstract away common functionality across V2.0 and V2.1
 * subHal interfaces. Much of the logic is common between the two versions and this allows users of
 * the classes to only care about the type used at initialization and then interact with either
 * version of the subHal interface without worrying about the type.
 */
class ISubHalWrapperBase {
  protected:
    using Event = ::android::hardware::sensors::V2_1::Event;
    using OperationMode = ::android::hardware::sensors::V1_0::OperationMode;
    using RateLevel = ::android::hardware::sensors::V1_0::RateLevel;
    using Result = ::android::hardware::sensors::V1_0::Result;
    using SensorInfo = ::android::hardware::sensors::V2_1::SensorInfo;
    using SharedMemInfo = ::android::hardware::sensors::V1_0::SharedMemInfo;

  public:
    virtual ~ISubHalWrapperBase() {}

    virtual bool supportsNewEvents() = 0;

    virtual Return<Result> initialize(V2_0::implementation::ISubHalCallback* callback,
                                      V2_0::implementation::IScopedWakelockRefCounter* refCounter,
                                      int32_t subHalIndex) = 0;

    virtual Return<void> getSensorsList(
            ::android::hardware::sensors::V2_1::ISensors::getSensorsList_2_1_cb _hidl_cb) = 0;

    virtual Return<Result> setOperationMode(OperationMode mode) = 0;

    virtual Return<Result> activate(int32_t sensorHandle, bool enabled) = 0;

    virtual Return<Result> batch(int32_t sensorHandle, int64_t samplingPeriodNs,
                                 int64_t maxReportLatencyNs) = 0;

    virtual Return<Result> flush(int32_t sensorHandle) = 0;

    virtual Return<Result> injectSensorData(const Event& event) = 0;

    virtual Return<void> registerDirectChannel(const SharedMemInfo& mem,
                                               ISensors::registerDirectChannel_cb _hidl_cb) = 0;

    virtual Return<Result> unregisterDirectChannel(int32_t channelHandle) = 0;

    virtual Return<void> configDirectReport(int32_t sensorHandle, int32_t channelHandle,
                                            RateLevel rate,
                                            ISensors::configDirectReport_cb _hidl_cb) = 0;

    virtual Return<void> debug(const hidl_handle& fd, const hidl_vec<hidl_string>& args) = 0;

    virtual const std::string getName() = 0;
};

template <typename T>
class SubHalWrapperBase : public ISubHalWrapperBase {
  public:
    SubHalWrapperBase(T* subHal) : mSubHal(subHal){};

    virtual bool supportsNewEvents() override { return false; }

    virtual Return<void> getSensorsList(
            ::android::hardware::sensors::V2_1::ISensors::getSensorsList_2_1_cb _hidl_cb) override {
        return mSubHal->getSensorsList(
                [&](const auto& list) { _hidl_cb(convertToNewSensorInfos(list)); });
    }

    Return<Result> setOperationMode(OperationMode mode) override {
        return mSubHal->setOperationMode(mode);
    }

    Return<Result> activate(int32_t sensorHandle, bool enabled) override {
        return mSubHal->activate(sensorHandle, enabled);
    }

    Return<Result> batch(int32_t sensorHandle, int64_t samplingPeriodNs,
                         int64_t maxReportLatencyNs) override {
        return mSubHal->batch(sensorHandle, samplingPeriodNs, maxReportLatencyNs);
    }

    Return<Result> flush(int32_t sensorHandle) override { return mSubHal->flush(sensorHandle); }

    virtual Return<Result> injectSensorData(const Event& event) override {
        return mSubHal->injectSensorData(convertToOldEvent(event));
    }

    Return<void> registerDirectChannel(const SharedMemInfo& mem,
                                       ISensors::registerDirectChannel_cb _hidl_cb) override {
        return mSubHal->registerDirectChannel(mem, _hidl_cb);
    }

    Return<Result> unregisterDirectChannel(int32_t channelHandle) override {
        return mSubHal->unregisterDirectChannel(channelHandle);
    }

    Return<void> configDirectReport(int32_t sensorHandle, int32_t channelHandle, RateLevel rate,
                                    ISensors::configDirectReport_cb _hidl_cb) override {
        return mSubHal->configDirectReport(sensorHandle, channelHandle, rate, _hidl_cb);
    }

    Return<void> debug(const hidl_handle& fd, const hidl_vec<hidl_string>& args) override {
        return mSubHal->debug(fd, args);
    }

    const std::string getName() override { return mSubHal->getName(); }

  protected:
    T* mSubHal;
};

class SubHalWrapperV2_0 : public SubHalWrapperBase<V2_0::implementation::ISensorsSubHal> {
  public:
    SubHalWrapperV2_0(V2_0::implementation::ISensorsSubHal* subHal) : SubHalWrapperBase(subHal){};

    Return<Result> initialize(V2_0::implementation::ISubHalCallback* callback,
                              V2_0::implementation::IScopedWakelockRefCounter* refCounter,
                              int32_t subHalIndex) override {
        return mSubHal->initialize(
                new V2_0::implementation::HalProxyCallbackV2_0(callback, refCounter, subHalIndex));
    }
};

class SubHalWrapperV2_1 : public SubHalWrapperBase<V2_1::implementation::ISensorsSubHal> {
  public:
    SubHalWrapperV2_1(V2_1::implementation::ISensorsSubHal* subHal) : SubHalWrapperBase(subHal) {}

    bool supportsNewEvents() override { return true; }

    virtual Return<void> getSensorsList(
            ::android::hardware::sensors::V2_1::ISensors::getSensorsList_2_1_cb _hidl_cb) override {
        return mSubHal->getSensorsList_2_1([&](const auto& list) { _hidl_cb(list); });
    }

    virtual Return<Result> injectSensorData(const Event& event) override {
        return mSubHal->injectSensorData_2_1(event);
    }

    Return<Result> initialize(V2_0::implementation::ISubHalCallback* callback,
                              V2_0::implementation::IScopedWakelockRefCounter* refCounter,
                              int32_t subHalIndex) override {
        return mSubHal->initialize(
                new V2_0::implementation::HalProxyCallbackV2_1(callback, refCounter, subHalIndex));
    }
};

}  // namespace implementation
}  // namespace V2_1
}  // namespace sensors
}  // namespace hardware
}  // namespace android
