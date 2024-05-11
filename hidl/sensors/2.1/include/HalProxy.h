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

#include "EventMessageQueueWrapper.h"
#include "HalProxyCallback.h"
#include "ISensorsCallbackWrapper.h"
#include "SubHalWrapper.h"
#include "V2_0/ScopedWakelock.h"
#include "V2_0/SubHal.h"
#include "V2_1/SubHal.h"
#include "WakeLockMessageQueueWrapper.h"
#include "convertV2_1.h"

#include <android/hardware/sensors/2.1/ISensors.h>
#include <android/hardware/sensors/2.1/types.h>
#include <fmq/MessageQueue.h>
#include <hardware_legacy/power.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

#include <atomic>
#include <condition_variable>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

namespace android {
namespace hardware {
namespace sensors {
namespace V2_1 {
namespace implementation {

/**
 * HalProxy is the main interface for Multi-HAL. It is responsible for managing  subHALs and
 * proxying function calls to/from the subHAL APIs from the sensors framework. It also manages any
 * wakelocks allocated through the IHalProxyCallback and manages posting events to the sensors
 * framework.
 */
class HalProxy : public V2_0::implementation::IScopedWakelockRefCounter,
                 public V2_0::implementation::ISubHalCallback {
  public:
    using Event = ::android::hardware::sensors::V2_1::Event;
    using OperationMode = ::android::hardware::sensors::V1_0::OperationMode;
    using RateLevel = ::android::hardware::sensors::V1_0::RateLevel;
    using Result = ::android::hardware::sensors::V1_0::Result;
    using SensorInfo = ::android::hardware::sensors::V2_1::SensorInfo;
    using SharedMemInfo = ::android::hardware::sensors::V1_0::SharedMemInfo;
    using IHalProxyCallbackV2_0 = V2_0::implementation::IHalProxyCallback;
    using IHalProxyCallbackV2_1 = V2_1::implementation::IHalProxyCallback;
    using ISensorsSubHalV2_0 = V2_0::implementation::ISensorsSubHal;
    using ISensorsSubHalV2_1 = V2_1::implementation::ISensorsSubHal;
    using ISensorsV2_0 = V2_0::ISensors;
    using ISensorsV2_1 = V2_1::ISensors;
    using HalProxyCallbackBase = V2_0::implementation::HalProxyCallbackBase;

    explicit HalProxy();
    // Test only constructor.
    explicit HalProxy(std::vector<ISensorsSubHalV2_0*>& subHalList);
    explicit HalProxy(std::vector<ISensorsSubHalV2_0*>& subHalList,
                      std::vector<ISensorsSubHalV2_1*>& subHalListV2_1);
    ~HalProxy();

    // Methods from ::android::hardware::sensors::V2_1::ISensors follow.
    Return<void> getSensorsList_2_1(ISensorsV2_1::getSensorsList_2_1_cb _hidl_cb);

    Return<Result> initialize_2_1(
            const ::android::hardware::MQDescriptorSync<V2_1::Event>& eventQueueDescriptor,
            const ::android::hardware::MQDescriptorSync<uint32_t>& wakeLockDescriptor,
            const sp<V2_1::ISensorsCallback>& sensorsCallback);

    Return<Result> injectSensorData_2_1(const Event& event);

    // Methods from ::android::hardware::sensors::V2_0::ISensors follow.
    Return<void> getSensorsList(ISensorsV2_0::getSensorsList_cb _hidl_cb);

    Return<Result> setOperationMode(OperationMode mode);

    Return<Result> activate(int32_t sensorHandle, bool enabled);

    Return<Result> initialize(
            const ::android::hardware::MQDescriptorSync<V1_0::Event>& eventQueueDescriptor,
            const ::android::hardware::MQDescriptorSync<uint32_t>& wakeLockDescriptor,
            const sp<V2_0::ISensorsCallback>& sensorsCallback);

    Return<Result> initializeCommon(std::unique_ptr<EventMessageQueueWrapperBase>& eventQueue,
                                    std::unique_ptr<WakeLockMessageQueueWrapperBase>& wakeLockQueue,
                                    const sp<ISensorsCallbackWrapperBase>& sensorsCallback);

    Return<Result> batch(int32_t sensorHandle, int64_t samplingPeriodNs,
                         int64_t maxReportLatencyNs);

    Return<Result> flush(int32_t sensorHandle);

    Return<Result> injectSensorData(const V1_0::Event& event);

    Return<void> registerDirectChannel(const SharedMemInfo& mem,
                                       ISensorsV2_0::registerDirectChannel_cb _hidl_cb);

    Return<Result> unregisterDirectChannel(int32_t channelHandle);

    Return<void> configDirectReport(int32_t sensorHandle, int32_t channelHandle, RateLevel rate,
                                    ISensorsV2_0::configDirectReport_cb _hidl_cb);

    Return<void> debug(const hidl_handle& fd, const hidl_vec<hidl_string>& args);

    Return<void> onDynamicSensorsConnected(const hidl_vec<SensorInfo>& dynamicSensorsAdded,
                                           int32_t subHalIndex) override;

    Return<void> onDynamicSensorsDisconnected(const hidl_vec<int32_t>& dynamicSensorHandlesRemoved,
                                              int32_t subHalIndex) override;

    void postEventsToMessageQueue(const std::vector<Event>& events, size_t numWakeupEvents,
                                  V2_0::implementation::ScopedWakelock wakelock) override;

    const SensorInfo& getSensorInfo(int32_t sensorHandle) override {
        return mSensors[sensorHandle];
    }

    bool areThreadsRunning() override { return mThreadsRun.load(); }

    // Below methods are from IScopedWakelockRefCounter interface
    bool incrementRefCountAndMaybeAcquireWakelock(size_t delta,
                                                  int64_t* timeoutStart = nullptr) override;

    void decrementRefCountAndMaybeReleaseWakelock(size_t delta, int64_t timeoutStart = -1) override;

    const std::map<int32_t, SensorInfo>& getSensors() { return mSensors; }

  private:
    using EventMessageQueueV2_1 = MessageQueue<V2_1::Event, kSynchronizedReadWrite>;
    using EventMessageQueueV2_0 = MessageQueue<V1_0::Event, kSynchronizedReadWrite>;
    using WakeLockMessageQueue = MessageQueue<uint32_t, kSynchronizedReadWrite>;

    /**
     * The Event FMQ where sensor events are written
     */
    std::unique_ptr<EventMessageQueueWrapperBase> mEventQueue;

    /**
     * The Wake Lock FMQ that is read to determine when the framework has handled WAKE_UP events
     */
    std::unique_ptr<WakeLockMessageQueueWrapperBase> mWakeLockQueue;

    /**
     * Event Flag to signal to the framework when sensor events are available to be read and to
     * interrupt event queue blocking write.
     */
    EventFlag* mEventQueueFlag = nullptr;

    //! Event Flag to signal internally that the wakelock queue should stop its blocking read.
    EventFlag* mWakelockQueueFlag = nullptr;

    /**
     * Callback to the sensors framework to inform it that new sensors have been added or removed.
     */
    sp<ISensorsCallbackWrapperBase> mDynamicSensorsCallback;

    /**
     * SubHal objects that have been saved from vendor dynamic libraries.
     */
    std::vector<std::shared_ptr<ISubHalWrapperBase>> mSubHalList;

    /**
     * Map of sensor handles to SensorInfo objects that contains the sensor info from subhals as
     * well as the modified sensor handle for the framework.
     *
     * The subhal index is encoded in the first byte of the sensor handle and the remaining
     * bytes are generated by the subhal to identify the sensor.
     */
    std::map<int32_t, SensorInfo> mSensors;

    //! Map of the dynamic sensors that have been added to halproxy.
    std::map<int32_t, SensorInfo> mDynamicSensors;

    //! The current operation mode for all subhals.
    OperationMode mCurrentOperationMode = OperationMode::NORMAL;

    //! The single subHal that supports directChannel reporting.
    std::shared_ptr<ISubHalWrapperBase> mDirectChannelSubHal;

    //! The timeout for each pending write on background thread for events.
    static const int64_t kPendingWriteTimeoutNs = 5 * INT64_C(1000000000) /* 5 seconds */;

    //! The bit mask used to get the subhal index from a sensor handle.
    static constexpr int32_t kSensorHandleSubHalIndexMask = 0xFF000000;

    /**
     * A FIFO queue of pairs of vector of events and the number of wakeup events in that vector
     * which are waiting to be written to the events fmq in the background thread.
     */
    std::queue<std::pair<std::vector<Event>, size_t>> mPendingWriteEventsQueue;

    //! The most events observed on the pending write events queue for debug purposes.
    size_t mMostEventsObservedPendingWriteEventsQueue = 0;

    //! The max number of events allowed in the pending write events queue
    static constexpr size_t kMaxSizePendingWriteEventsQueue = 100000;

    //! The number of events in the pending write events queue
    size_t mSizePendingWriteEventsQueue = 0;

    //! The mutex protecting writing to the fmq and the pending events queue
    std::mutex mEventQueueWriteMutex;

    //! The condition variable waiting on pending write events to stack up
    std::condition_variable mEventQueueWriteCV;

    //! The thread object ptr that handles pending writes
    std::thread mPendingWritesThread;

    //! The thread object that handles wakelocks
    std::thread mWakelockThread;

    //! The bool indicating whether to end the threads started in initialize
    std::atomic_bool mThreadsRun = true;

    //! The mutex protecting access to the dynamic sensors added and removed methods.
    std::mutex mDynamicSensorsMutex;

    // WakelockRefCount membar vars below

    //! The mutex protecting the wakelock refcount and subsequent wakelock releases and
    //! acquisitions
    std::recursive_mutex mWakelockMutex;

    std::condition_variable_any mWakelockCV;

    //! The refcount of how many ScopedWakelocks and pending wakeup events are active
    size_t mWakelockRefCount = 0;

    int64_t mWakelockTimeoutStartTime = V2_0::implementation::getTimeNow();

    int64_t mWakelockTimeoutResetTime = V2_0::implementation::getTimeNow();

    const char* kWakelockName = "SensorsHAL_WAKEUP";

    /**
     * Initialize the list of SubHal objects in mSubHalList by reading from dynamic libraries
     * listed in a config file.
     */
    void initializeSubHalListFromConfigFile(const char* configFileName);

    /**
     * Initialize the HalProxyCallback vector using the list of subhals.
     */
    void initializeSubHalCallbacks();

    /**
     * Initialize the list of SensorInfo objects in mSensorList by getting sensors from each
     * subhal.
     */
    void initializeSensorList();

    /**
     * Try using the default include directories as well as the directories defined in
     * kSubHalShareObjectLocations to get a handle for dlsym for a subhal.
     *
     * @param filename The file name to search for.
     *
     * @return The handle or nullptr if search failed.
     */
    void* getHandleForSubHalSharedObject(const std::string& filename);

    /**
     * Calls the helper methods that all ctors use.
     */
    void init();

    /**
     * Stops all threads by setting the threads running flag to false and joining to them.
     */
    void stopThreads();

    /**
     * Disable all the sensors observed by the HalProxy.
     */
    void disableAllSensors();

    /**
     * Starts the thread that handles pending writes to event fmq.
     *
     * @param halProxy The HalProxy object pointer.
     */
    static void startPendingWritesThread(HalProxy* halProxy);

    //! Handles the pending writes on events to eventqueue.
    void handlePendingWrites();

    /**
     * Starts the thread that handles decrementing the ref count on wakeup events processed by the
     * framework and timing out wakelocks.
     *
     * @param halProxy The HalProxy object pointer.
     */
    static void startWakelockThread(HalProxy* halProxy);

    //! Handles the wakelocks.
    void handleWakelocks();

    /**
     * @param timeLeft The variable that should be set to the timeleft before timeout will occur or
     * unmodified if timeout occurred.
     *
     * @return true if the shared wakelock has been held passed the timeout and should be released
     */
    bool sharedWakelockDidTimeout(int64_t* timeLeft);

    /**
     * Reset all the member variables associated with the wakelock ref count and maybe release
     * the shared wakelock.
     */
    void resetSharedWakelock();

    /**
     * Clear direct channel flags if the HalProxy has already chosen a subhal as its direct channel
     * subhal. Set the directChannelSubHal pointer to the subHal passed in if this is the first
     * direct channel enabled sensor seen.
     *
     * @param sensorInfo The SensorInfo object that may be altered to have direct channel support
     *    disabled.
     * @param subHal The subhal pointer that the current sensorInfo object came from.
     */
    void setDirectChannelFlags(SensorInfo* sensorInfo, std::shared_ptr<ISubHalWrapperBase> subHal);

    /*
     * Get the subhal pointer which can be found by indexing into the mSubHalList vector
     * using the index from the first byte of sensorHandle.
     *
     * @param sensorHandle The handle used to identify a sensor in one of the subhals.
     */
    std::shared_ptr<ISubHalWrapperBase> getSubHalForSensorHandle(int32_t sensorHandle);

    /**
     * Checks that sensorHandle's subhal index byte is within bounds of mSubHalList.
     *
     * @param sensorHandle The sensor handle to check.
     *
     * @return true if sensorHandles's subhal index byte is valid.
     */
    bool isSubHalIndexValid(int32_t sensorHandle);

    /**
     * Count the number of wakeup events in the first n events of the vector.
     *
     * @param events The vector of Event objects.
     * @param n The end index not inclusive of events to consider.
     *
     * @return The number of wakeup events of the considered events.
     */
    size_t countNumWakeupEvents(const std::vector<Event>& events, size_t n);

    /*
     * Clear out the subhal index bytes from a sensorHandle.
     *
     * @param sensorHandle The sensor handle to modify.
     *
     * @return The modified version of the sensor handle.
     */
    static int32_t clearSubHalIndex(int32_t sensorHandle);

    /**
     * @param sensorHandle The sensor handle to modify.
     *
     * @return true if subHalIndex byte of sensorHandle is zeroed.
     */
    static bool subHalIndexIsClear(int32_t sensorHandle);
};

/**
 * Since a newer HAL can't masquerade as a older HAL, IHalProxy enables the HalProxy to be compiled
 * either for HAL 2.0 or HAL 2.1 depending on the build configuration.
 */
template <class ISensorsVersion>
class IHalProxy : public HalProxy, public ISensorsVersion {
    Return<void> getSensorsList(ISensorsV2_0::getSensorsList_cb _hidl_cb) override {
        return HalProxy::getSensorsList(_hidl_cb);
    }

    Return<Result> setOperationMode(OperationMode mode) override {
        return HalProxy::setOperationMode(mode);
    }

    Return<Result> activate(int32_t sensorHandle, bool enabled) override {
        return HalProxy::activate(sensorHandle, enabled);
    }

    Return<Result> initialize(
            const ::android::hardware::MQDescriptorSync<V1_0::Event>& eventQueueDescriptor,
            const ::android::hardware::MQDescriptorSync<uint32_t>& wakeLockDescriptor,
            const sp<V2_0::ISensorsCallback>& sensorsCallback) override {
        return HalProxy::initialize(eventQueueDescriptor, wakeLockDescriptor, sensorsCallback);
    }

    Return<Result> batch(int32_t sensorHandle, int64_t samplingPeriodNs,
                         int64_t maxReportLatencyNs) override {
        return HalProxy::batch(sensorHandle, samplingPeriodNs, maxReportLatencyNs);
    }

    Return<Result> flush(int32_t sensorHandle) override { return HalProxy::flush(sensorHandle); }

    Return<Result> injectSensorData(const V1_0::Event& event) override {
        return HalProxy::injectSensorData(event);
    }

    Return<void> registerDirectChannel(const SharedMemInfo& mem,
                                       ISensorsV2_0::registerDirectChannel_cb _hidl_cb) override {
        return HalProxy::registerDirectChannel(mem, _hidl_cb);
    }

    Return<Result> unregisterDirectChannel(int32_t channelHandle) override {
        return HalProxy::unregisterDirectChannel(channelHandle);
    }

    Return<void> configDirectReport(int32_t sensorHandle, int32_t channelHandle, RateLevel rate,
                                    ISensorsV2_0::configDirectReport_cb _hidl_cb) override {
        return HalProxy::configDirectReport(sensorHandle, channelHandle, rate, _hidl_cb);
    }

    Return<void> debug(const hidl_handle& fd, const hidl_vec<hidl_string>& args) override {
        return HalProxy::debug(fd, args);
    }
};

class HalProxyV2_0 : public IHalProxy<V2_0::ISensors> {};

class HalProxyV2_1 : public IHalProxy<V2_1::ISensors> {
    Return<void> getSensorsList_2_1(ISensorsV2_1::getSensorsList_2_1_cb _hidl_cb) override {
        return HalProxy::getSensorsList_2_1(_hidl_cb);
    }

    Return<Result> initialize_2_1(
            const ::android::hardware::MQDescriptorSync<V2_1::Event>& eventQueueDescriptor,
            const ::android::hardware::MQDescriptorSync<uint32_t>& wakeLockDescriptor,
            const sp<V2_1::ISensorsCallback>& sensorsCallback) override {
        return HalProxy::initialize_2_1(eventQueueDescriptor, wakeLockDescriptor, sensorsCallback);
    }

    Return<Result> injectSensorData_2_1(const Event& event) override {
        return HalProxy::injectSensorData_2_1(event);
    }
};

}  // namespace implementation
}  // namespace V2_1
}  // namespace sensors
}  // namespace hardware
}  // namespace android
