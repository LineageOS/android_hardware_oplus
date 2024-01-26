/*
 * Copyright 2021 The Android Open Source Project
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

#include <aidl/android/hardware/power/BnPowerHintSession.h>
#include <aidl/android/hardware/power/SessionHint.h>
#include <aidl/android/hardware/power/WorkDuration.h>
#include <utils/Looper.h>
#include <utils/Thread.h>

#include <mutex>
#include <unordered_map>

namespace aidl {
namespace google {
namespace hardware {
namespace power {
namespace impl {
namespace pixel {

using aidl::android::hardware::power::BnPowerHintSession;
using aidl::android::hardware::power::SessionHint;
using aidl::android::hardware::power::WorkDuration;
using ::android::Message;
using ::android::MessageHandler;
using ::android::sp;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::chrono::steady_clock;
using std::chrono::time_point;

struct AppHintDesc {
    AppHintDesc(int32_t tgid, int32_t uid, std::vector<int32_t> threadIds)
        : tgid(tgid),
          uid(uid),
          threadIds(std::move(threadIds)),
          duration(0LL),
          current_min(0),
          is_active(true),
          update_count(0),
          integral_error(0),
          previous_error(0) {}
    std::string toString() const;
    const int32_t tgid;
    const int32_t uid;
    std::vector<int32_t> threadIds;
    nanoseconds duration;
    int current_min;
    // status
    std::atomic<bool> is_active;
    // pid
    uint64_t update_count;
    int64_t integral_error;
    int64_t previous_error;
};

class PowerHintSession : public BnPowerHintSession {
  public:
    explicit PowerHintSession(int32_t tgid, int32_t uid, const std::vector<int32_t> &threadIds,
                              int64_t durationNanos);
    ~PowerHintSession();
    ndk::ScopedAStatus close() override;
    ndk::ScopedAStatus pause() override;
    ndk::ScopedAStatus resume() override;
    ndk::ScopedAStatus updateTargetWorkDuration(int64_t targetDurationNanos) override;
    ndk::ScopedAStatus reportActualWorkDuration(
            const std::vector<WorkDuration> &actualDurations) override;
    ndk::ScopedAStatus sendHint(SessionHint hint) override;
    ndk::ScopedAStatus setThreads(const std::vector<int32_t> &threadIds) override;
    bool isActive();
    bool isTimeout();
    void setStale();
    // Is this hint session for a user application
    bool isAppSession();
    const std::vector<int> &getTidList() const;
    int getUclampMin();
    void dumpToStream(std::ostream &stream);

    // Disable any temporary boost and return to normal operation. It does not
    // reset the actual uclamp value, and relies on the caller to do so, to
    // prevent double-setting. Returns true if it actually disabled an active boost
    bool disableTemporaryBoost();

  private:
    class SessionTimerHandler : public MessageHandler {
      public:
        SessionTimerHandler(PowerHintSession *session, std::string name)
            : mSession(session), mIsSessionDead(false), mName(name) {}
        void updateTimer(nanoseconds delay);
        void handleMessage(const Message &message) override;
        void setSessionDead();
        virtual void onTimeout() = 0;

      protected:
        PowerHintSession *mSession;
        std::mutex mClosedLock;
        std::mutex mMessageLock;
        std::atomic<time_point<steady_clock>> mTimeout;
        bool mIsSessionDead;
        const std::string mName;
    };

    class StaleTimerHandler : public SessionTimerHandler {
      public:
        StaleTimerHandler(PowerHintSession *session) : SessionTimerHandler(session, "stale") {}
        void updateTimer();
        void onTimeout() override;
    };

    class BoostTimerHandler : public SessionTimerHandler {
      public:
        BoostTimerHandler(PowerHintSession *session) : SessionTimerHandler(session, "boost") {}
        void onTimeout() override;
    };

  private:
    void updateUniveralBoostMode();
    int setSessionUclampMin(int32_t min, bool resetStale = true);
    void tryToSendPowerHint(std::string hint);
    int64_t convertWorkDurationToBoostByPid(const std::vector<WorkDuration> &actualDurations);
    void traceSessionVal(char const *identifier, int64_t val) const;
    AppHintDesc *mDescriptor = nullptr;
    sp<StaleTimerHandler> mStaleTimerHandler;
    sp<BoostTimerHandler> mBoostTimerHandler;
    std::atomic<time_point<steady_clock>> mLastUpdatedTime;
    sp<MessageHandler> mPowerManagerHandler;
    std::mutex mSessionLock;
    std::atomic<bool> mSessionClosed = false;
    std::string mIdString;
    // Used when setting a temporary boost value to hold the true boost
    std::atomic<std::optional<int>> mNextUclampMin;
    // To cache the status of whether ADPF hints are supported.
    std::unordered_map<std::string, std::optional<bool>> mSupportedHints;
    // Last session hint sent, used for logging
    int mLastHintSent = -1;
};

}  // namespace pixel
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace google
}  // namespace aidl
