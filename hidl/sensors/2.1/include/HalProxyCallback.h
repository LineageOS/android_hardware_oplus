/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "V2_0/ScopedWakelock.h"
#include "V2_0/SubHal.h"
#include "V2_1/SubHal.h"
#include "convertV2_1.h"

#include <android/hardware/sensors/2.1/ISensors.h>
#include <android/hardware/sensors/2.1/types.h>
#include <log/log.h>

namespace android {
namespace hardware {
namespace sensors {
namespace V2_0 {
namespace implementation {

/**
 * Interface used to communicate with the HalProxy when subHals interact with their provided
 * callback.
 */
class ISubHalCallback {
  public:
    virtual ~ISubHalCallback() {}

    // Below methods from ::android::hardware::sensors::V2_0::ISensorsCallback with a minor change
    // to pass in the sub-HAL index. While the above methods are invoked from the sensors framework
    // via the binder, these methods are invoked from a callback provided to sub-HALs inside the
    // same process as the HalProxy, but potentially running on different threads.
    virtual Return<void> onDynamicSensorsConnected(
            const hidl_vec<V2_1::SensorInfo>& dynamicSensorsAdded, int32_t subHalIndex) = 0;

    virtual Return<void> onDynamicSensorsDisconnected(
            const hidl_vec<int32_t>& dynamicSensorHandlesRemoved, int32_t subHalIndex) = 0;

    /**
     * Post events to the event message queue if there is room to write them. Otherwise post the
     * remaining events to a background thread for a blocking write with a kPendingWriteTimeoutNs
     * timeout.
     *
     * @param events The list of events to post to the message queue.
     * @param numWakeupEvents The number of wakeup events in events.
     * @param wakelock The wakelock associated with this post of events.
     */
    virtual void postEventsToMessageQueue(const std::vector<V2_1::Event>& events,
                                          size_t numWakeupEvents,
                                          V2_0::implementation::ScopedWakelock wakelock) = 0;

    /**
     * Get the sensor info associated with that sensorHandle.
     *
     * @param sensorHandle The sensor handle.
     *
     * @return The sensor info object in the mapping.
     */
    virtual const V2_1::SensorInfo& getSensorInfo(int32_t sensorHandle) = 0;

    virtual bool areThreadsRunning() = 0;
};

/**
 * Callback class given to subhals that allows the HalProxy to know which subhal a given invocation
 * is coming from.
 */
class HalProxyCallbackBase : public VirtualLightRefBase {
  public:
    HalProxyCallbackBase(ISubHalCallback* callback,
                         V2_0::implementation::IScopedWakelockRefCounter* refCounter,
                         int32_t subHalIndex)
        : mCallback(callback), mRefCounter(refCounter), mSubHalIndex(subHalIndex) {}

    void postEvents(const std::vector<V2_1::Event>& events,
                    V2_0::implementation::ScopedWakelock wakelock);

    V2_0::implementation::ScopedWakelock createScopedWakelock(bool lock);

  protected:
    ISubHalCallback* mCallback;
    V2_0::implementation::IScopedWakelockRefCounter* mRefCounter;
    int32_t mSubHalIndex;

  private:
    std::vector<V2_1::Event> processEvents(const std::vector<V2_1::Event>& events,
                                           size_t* numWakeupEvents) const;
};

class HalProxyCallbackV2_0 : public HalProxyCallbackBase,
                             public V2_0::implementation::IHalProxyCallback {
  public:
    HalProxyCallbackV2_0(ISubHalCallback* callback,
                         V2_0::implementation::IScopedWakelockRefCounter* refCounter,
                         int32_t subHalIndex)
        : HalProxyCallbackBase(callback, refCounter, subHalIndex) {}

    Return<void> onDynamicSensorsConnected(
            const hidl_vec<V1_0::SensorInfo>& dynamicSensorsAdded) override {
        return mCallback->onDynamicSensorsConnected(
                V2_1::implementation::convertToNewSensorInfos(dynamicSensorsAdded), mSubHalIndex);
    }

    Return<void> onDynamicSensorsDisconnected(
            const hidl_vec<int32_t>& dynamicSensorHandlesRemoved) override {
        return mCallback->onDynamicSensorsDisconnected(dynamicSensorHandlesRemoved, mSubHalIndex);
    }

    void postEvents(const std::vector<V1_0::Event>& events,
                    V2_0::implementation::ScopedWakelock wakelock) override {
        HalProxyCallbackBase::postEvents(V2_1::implementation::convertToNewEvents(events),
                                         std::move(wakelock));
    }

    V2_0::implementation::ScopedWakelock createScopedWakelock(bool lock) override {
        return HalProxyCallbackBase::createScopedWakelock(lock);
    }
};

class HalProxyCallbackV2_1 : public HalProxyCallbackBase,
                             public V2_1::implementation::IHalProxyCallback {
  public:
    HalProxyCallbackV2_1(ISubHalCallback* callback,
                         V2_0::implementation::IScopedWakelockRefCounter* refCounter,
                         int32_t subHalIndex)
        : HalProxyCallbackBase(callback, refCounter, subHalIndex) {}

    Return<void> onDynamicSensorsConnected_2_1(
            const hidl_vec<V2_1::SensorInfo>& dynamicSensorsAdded) override {
        return mCallback->onDynamicSensorsConnected(dynamicSensorsAdded, mSubHalIndex);
    }

    Return<void> onDynamicSensorsConnected(
            const hidl_vec<V1_0::SensorInfo>& /* dynamicSensorsAdded */) override {
        LOG_ALWAYS_FATAL("Old dynamic sensors method can't be used");
        return Void();
    }

    Return<void> onDynamicSensorsDisconnected(
            const hidl_vec<int32_t>& dynamicSensorHandlesRemoved) override {
        return mCallback->onDynamicSensorsDisconnected(dynamicSensorHandlesRemoved, mSubHalIndex);
    }

    void postEvents(const std::vector<V2_1::Event>& events,
                    V2_0::implementation::ScopedWakelock wakelock) override {
        return HalProxyCallbackBase::postEvents(events, std::move(wakelock));
    }

    V2_0::implementation::ScopedWakelock createScopedWakelock(bool lock) override {
        return HalProxyCallbackBase::createScopedWakelock(lock);
    }
};

}  // namespace implementation
}  // namespace V2_0
}  // namespace sensors
}  // namespace hardware
}  // namespace android