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

#include <android/hardware/sensors/2.0/types.h>

namespace android {
namespace hardware {
namespace sensors {
namespace V2_0 {
namespace implementation {

using ::android::hardware::sensors::V2_0::SensorTimeout;

const int64_t kWakelockTimeoutNs =
        static_cast<int64_t>(SensorTimeout::WAKE_LOCK_SECONDS) * INT64_C(1000000000);

int64_t getTimeNow();

class IScopedWakelockRefCounter : public RefBase {
  public:
    /**
     * Increment the wakelock ref count and maybe acquire the shared wakelock if incrementing
     * from 0 then return the time of incrementing back to caller.
     *
     * @param delta The amount to change ref count by.
     * @param timeoutStart The ptr to the timestamp in ns that the increment occurred which will be
     *        set in the function or nullptr if not specified.
     *
     * @return true if successfully incremented the wakelock ref count.
     */
    virtual bool incrementRefCountAndMaybeAcquireWakelock(size_t delta,
                                                          int64_t* timeoutStart = nullptr) = 0;
    /**
     * Decrement the wakelock ref count and maybe release wakelock if ref count ends up 0.
     *
     * @param delta The amount to change ref count by.
     * @param timeoutStart The timestamp in ns that the calling context kept track of when
     *        incrementing the ref count or -1 by default
     */
    virtual void decrementRefCountAndMaybeReleaseWakelock(size_t delta,
                                                          int64_t timeoutStart = -1) = 0;
    // Virtual dtor needed for compilation success
    virtual ~IScopedWakelockRefCounter(){};
};

/**
 * Wrapper around wake lock acquisition functions (acquire/release_wake_lock) that provides a
 * RAII-style mechanism for keeping a wake lock held for the duration of a scoped block.
 * When a ScopedWakelock is created, it increments the reference count stored in the HalProxy
 * for the sub-HALs specific wake lock, acquiring the wake lock if necessary. When the object goes
 * out of scope, the ref count is decremented, potentially releasing the wake lock if no other
 * references to the wake lock exist.
 *
 * This class is allocated through the createScopedWakelock callback inside the IHalProxyCallback
 * provided to sub-HALs during initialization and should be used for all wake lock acquisition
 * inside of the sub-HAL to ensure wake locks are not held indefinitely.
 *
 * The most prevalent use case for this class will be for posting events to the framework through
 * the postEvents HalProxy callback. The expectation is that sub-HALs will create this
 * ScopedWakelock through the createScopedWakelock upon receiving a sensor events. The lock boolean
 * provided to createScopedWakelock will be set the according to whether the sensor events are
 * from wakeup sensors. Then, the sub-HAL will perform any processing necessary before invoking the
 * postEvents callback passing in the previously created ScopedWakelock. At this point, ownership
 * of the object will be passed to the HalProxy that will then be responsible for ensuring any
 * wake locks continue to be held, if necessary.
 */
class ScopedWakelock {
  public:
    ScopedWakelock(ScopedWakelock&& other);
    ScopedWakelock& operator=(ScopedWakelock&& other);
    virtual ~ScopedWakelock();

    bool isLocked() const { return mLocked; }

  private:
    friend class HalProxyCallbackBase;
    friend class ScopedWakelockTest;
    IScopedWakelockRefCounter* mRefCounter;
    int64_t mCreatedAtTimeNs;
    bool mLocked;
    ScopedWakelock(IScopedWakelockRefCounter* refCounter, bool locked);
    ScopedWakelock(const ScopedWakelock&) = delete;
    ScopedWakelock& operator=(const ScopedWakelock&) = delete;
};

}  // namespace implementation
}  // namespace V2_0
}  // namespace sensors
}  // namespace hardware
}  // namespace android