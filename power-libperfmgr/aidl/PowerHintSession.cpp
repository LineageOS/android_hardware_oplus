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

#define LOG_TAG "powerhal-libperfmgr"
#define ATRACE_TAG (ATRACE_TAG_POWER | ATRACE_TAG_HAL)

#include "PowerHintSession.h"

#include <android-base/logging.h>
#include <android-base/parsedouble.h>
#include <android-base/properties.h>
#include <android-base/stringprintf.h>
#include <perfmgr/AdpfConfig.h>
#include <private/android_filesystem_config.h>
#include <sys/syscall.h>
#include <time.h>
#include <utils/Trace.h>

#include <atomic>

#include "PowerSessionManager.h"

namespace aidl {
namespace google {
namespace hardware {
namespace power {
namespace impl {
namespace pixel {

using ::android::base::StringPrintf;
using ::android::perfmgr::AdpfConfig;
using ::android::perfmgr::HintManager;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;

namespace {

static inline int64_t ns_to_100us(int64_t ns) {
    return ns / 100000;
}

}  // namespace

int64_t PowerHintSession::convertWorkDurationToBoostByPid(
        const std::vector<WorkDuration> &actualDurations) {
    std::shared_ptr<AdpfConfig> adpfConfig = HintManager::GetInstance()->GetAdpfProfile();
    const nanoseconds &targetDuration = mDescriptor->duration;
    int64_t &integral_error = mDescriptor->integral_error;
    int64_t &previous_error = mDescriptor->previous_error;
    uint64_t samplingWindowP = adpfConfig->mSamplingWindowP;
    uint64_t samplingWindowI = adpfConfig->mSamplingWindowI;
    uint64_t samplingWindowD = adpfConfig->mSamplingWindowD;
    int64_t targetDurationNanos = (int64_t)targetDuration.count();
    int64_t length = actualDurations.size();
    int64_t p_start =
            samplingWindowP == 0 || samplingWindowP > length ? 0 : length - samplingWindowP;
    int64_t i_start =
            samplingWindowI == 0 || samplingWindowI > length ? 0 : length - samplingWindowI;
    int64_t d_start =
            samplingWindowD == 0 || samplingWindowD > length ? 0 : length - samplingWindowD;
    int64_t dt = ns_to_100us(targetDurationNanos);
    int64_t err_sum = 0;
    int64_t derivative_sum = 0;
    for (int64_t i = std::min({p_start, i_start, d_start}); i < length; i++) {
        int64_t actualDurationNanos = actualDurations[i].durationNanos;
        if (std::abs(actualDurationNanos) > targetDurationNanos * 20) {
            ALOGW("The actual duration is way far from the target (%" PRId64 " >> %" PRId64 ")",
                  actualDurationNanos, targetDurationNanos);
        }
        // PID control algorithm
        int64_t error = ns_to_100us(actualDurationNanos - targetDurationNanos);
        if (i >= d_start) {
            derivative_sum += error - previous_error;
        }
        if (i >= p_start) {
            err_sum += error;
        }
        if (i >= i_start) {
            integral_error += error * dt;
            integral_error = std::min(adpfConfig->getPidIHighDivI(), integral_error);
            integral_error = std::max(adpfConfig->getPidILowDivI(), integral_error);
        }
        previous_error = error;
    }
    int64_t pOut = static_cast<int64_t>((err_sum > 0 ? adpfConfig->mPidPo : adpfConfig->mPidPu) *
                                        err_sum / (length - p_start));
    int64_t iOut = static_cast<int64_t>(adpfConfig->mPidI * integral_error);
    int64_t dOut =
            static_cast<int64_t>((derivative_sum > 0 ? adpfConfig->mPidDo : adpfConfig->mPidDu) *
                                 derivative_sum / dt / (length - d_start));

    int64_t output = pOut + iOut + dOut;
    if (ATRACE_ENABLED()) {
        traceSessionVal("pid.err", err_sum / (length - p_start));
        traceSessionVal("pid.integral", integral_error);
        traceSessionVal("pid.derivative", derivative_sum / dt / (length - d_start));
        traceSessionVal("pid.pOut", pOut);
        traceSessionVal("pid.iOut", iOut);
        traceSessionVal("pid.dOut", dOut);
        traceSessionVal("pid.output", output);
    }
    return output;
}

PowerHintSession::PowerHintSession(int32_t tgid, int32_t uid, const std::vector<int32_t> &threadIds,
                                   int64_t durationNanos)
    : mStaleTimerHandler(sp<StaleTimerHandler>::make(this)),
      mBoostTimerHandler(sp<BoostTimerHandler>::make(this)) {
    mDescriptor = new AppHintDesc(tgid, uid, threadIds);
    mDescriptor->duration = std::chrono::nanoseconds(durationNanos);
    mIdString = StringPrintf("%" PRId32 "-%" PRId32 "-%" PRIxPTR, mDescriptor->tgid,
                             mDescriptor->uid, reinterpret_cast<uintptr_t>(this) & 0xffff);

    mPowerManagerHandler = PowerSessionManager::getInstance();
    mLastUpdatedTime.store(std::chrono::steady_clock::now());
    if (ATRACE_ENABLED()) {
        traceSessionVal("target", mDescriptor->duration.count());
        traceSessionVal("active", mDescriptor->is_active.load());
    }
    PowerSessionManager::getInstance()->addPowerSession(this);
    // init boost
    sendHint(SessionHint::CPU_LOAD_RESET);
    ALOGV("PowerHintSession created: %s", mDescriptor->toString().c_str());
}

PowerHintSession::~PowerHintSession() {
    close();
    ALOGV("PowerHintSession deleted: %s", mDescriptor->toString().c_str());
    if (ATRACE_ENABLED()) {
        traceSessionVal("target", 0);
        traceSessionVal("actl_last", 0);
        traceSessionVal("active", 0);
    }
    delete mDescriptor;
}

void PowerHintSession::traceSessionVal(char const *identifier, int64_t val) const {
    ATRACE_INT(StringPrintf("adpf.%s-%s", mIdString.c_str(), identifier).c_str(), val);
}

bool PowerHintSession::isAppSession() {
    // Check if uid is in range reserved for applications
    return mDescriptor->uid >= AID_APP_START;
}

void PowerHintSession::updateUniveralBoostMode() {
    if (!isAppSession()) {
        return;
    }
    if (ATRACE_ENABLED()) {
        const std::string tag = StringPrintf("%s:updateUniveralBoostMode()", mIdString.c_str());
        ATRACE_BEGIN(tag.c_str());
    }
    PowerHintMonitor::getInstance()->getLooper()->sendMessage(mPowerManagerHandler, NULL);
    if (ATRACE_ENABLED()) {
        ATRACE_END();
    }
}

void PowerHintSession::tryToSendPowerHint(std::string hint) {
    if (!mSupportedHints[hint].has_value()) {
        mSupportedHints[hint] = HintManager::GetInstance()->IsHintSupported(hint);
    }
    if (mSupportedHints[hint].value()) {
        HintManager::GetInstance()->DoHint(hint);
    }
}

int PowerHintSession::setSessionUclampMin(int32_t min, bool resetStale) {
    {
        std::lock_guard<std::mutex> guard(mSessionLock);
        mDescriptor->current_min = min;
    }
    if (min != 0 && resetStale) {
        mStaleTimerHandler->updateTimer();
    }
    PowerSessionManager::getInstance()->setUclampMin(this, min);

    if (ATRACE_ENABLED()) {
        traceSessionVal("min", min);
    }
    return 0;
}

int PowerHintSession::getUclampMin() {
    return mDescriptor->current_min;
}

void PowerHintSession::dumpToStream(std::ostream &stream) {
    stream << "ID.Min.Act.Timeout(" << mIdString;
    stream << ", " << mDescriptor->current_min;
    stream << ", " << mDescriptor->is_active.load();
    stream << ", " << isTimeout() << ")";
}

ndk::ScopedAStatus PowerHintSession::pause() {
    if (mSessionClosed) {
        ALOGE("Error: session is dead");
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    if (!mDescriptor->is_active.load())
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    // Reset to default uclamp value.
    mDescriptor->is_active.store(false);
    setStale();
    if (ATRACE_ENABLED()) {
        traceSessionVal("active", mDescriptor->is_active.load());
    }
    updateUniveralBoostMode();
    PowerSessionManager::getInstance()->removeThreadsFromPowerSession(this);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PowerHintSession::resume() {
    if (mSessionClosed) {
        ALOGE("Error: session is dead");
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    if (mDescriptor->is_active.load())
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    mDescriptor->is_active.store(true);
    PowerSessionManager::getInstance()->addThreadsFromPowerSession(this);
    // resume boost
    setSessionUclampMin(mDescriptor->current_min);
    if (ATRACE_ENABLED()) {
        traceSessionVal("active", mDescriptor->is_active.load());
    }
    updateUniveralBoostMode();
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PowerHintSession::close() {
    bool sessionClosedExpectedToBe = false;
    if (!mSessionClosed.compare_exchange_strong(sessionClosedExpectedToBe, true)) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    // Remove the session from PowerSessionManager first to avoid racing.
    PowerSessionManager::getInstance()->removePowerSession(this);
    mStaleTimerHandler->setSessionDead();
    mBoostTimerHandler->setSessionDead();
    setSessionUclampMin(0);
    mDescriptor->is_active.store(false);
    updateUniveralBoostMode();
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PowerHintSession::updateTargetWorkDuration(int64_t targetDurationNanos) {
    if (mSessionClosed) {
        ALOGE("Error: session is dead");
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    if (targetDurationNanos <= 0) {
        ALOGE("Error: targetDurationNanos(%" PRId64 ") should bigger than 0", targetDurationNanos);
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }
    targetDurationNanos =
            targetDurationNanos * HintManager::GetInstance()->GetAdpfProfile()->mTargetTimeFactor;
    ALOGV("update target duration: %" PRId64 " ns", targetDurationNanos);

    mDescriptor->duration = std::chrono::nanoseconds(targetDurationNanos);
    if (ATRACE_ENABLED()) {
        traceSessionVal("target", mDescriptor->duration.count());
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PowerHintSession::reportActualWorkDuration(
        const std::vector<WorkDuration> &actualDurations) {
    if (mSessionClosed) {
        ALOGE("Error: session is dead");
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    if (mDescriptor->duration.count() == 0LL) {
        ALOGE("Expect to call updateTargetWorkDuration() first.");
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    if (actualDurations.size() == 0) {
        ALOGE("Error: duration.size() shouldn't be %zu.", actualDurations.size());
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }
    if (!mDescriptor->is_active.load()) {
        ALOGE("Error: shouldn't report duration during pause state.");
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    std::shared_ptr<AdpfConfig> adpfConfig = HintManager::GetInstance()->GetAdpfProfile();
    mDescriptor->update_count++;
    bool isFirstFrame = isTimeout();
    if (ATRACE_ENABLED()) {
        traceSessionVal("batch_size", actualDurations.size());
        traceSessionVal("actl_last", actualDurations.back().durationNanos);
        traceSessionVal("target", mDescriptor->duration.count());
        traceSessionVal("hint.count", mDescriptor->update_count);
        traceSessionVal("hint.overtime",
                        actualDurations.back().durationNanos - mDescriptor->duration.count() > 0);
    }

    mLastUpdatedTime.store(std::chrono::steady_clock::now());
    if (isFirstFrame) {
        if (isAppSession()) {
            tryToSendPowerHint("ADPF_FIRST_FRAME");
        }
        updateUniveralBoostMode();
    }

    disableTemporaryBoost();

    if (!adpfConfig->mPidOn) {
        setSessionUclampMin(adpfConfig->mUclampMinHigh);
        return ndk::ScopedAStatus::ok();
    }

    int64_t output = convertWorkDurationToBoostByPid(actualDurations);

    /* apply to all the threads in the group */
    int next_min = std::min(static_cast<int>(adpfConfig->mUclampMinHigh),
                            mDescriptor->current_min + static_cast<int>(output));
    next_min = std::max(static_cast<int>(adpfConfig->mUclampMinLow), next_min);
    setSessionUclampMin(next_min);

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PowerHintSession::sendHint(SessionHint hint) {
    if (mSessionClosed) {
        ALOGE("Error: session is dead");
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    disableTemporaryBoost();
    std::shared_ptr<AdpfConfig> adpfConfig = HintManager::GetInstance()->GetAdpfProfile();
    switch (hint) {
        case SessionHint::CPU_LOAD_UP:
            mNextUclampMin.store(mDescriptor->current_min);
            mBoostTimerHandler->updateTimer(mDescriptor->duration * 2);
            setSessionUclampMin(adpfConfig->mUclampMinHigh);
            break;
        case SessionHint::CPU_LOAD_DOWN:
            setSessionUclampMin(adpfConfig->mUclampMinLow);
            break;
        case SessionHint::CPU_LOAD_RESET:
            mNextUclampMin.store(std::max(adpfConfig->mUclampMinInit,
                                          static_cast<uint32_t>(mDescriptor->current_min)));
            mBoostTimerHandler->updateTimer(duration_cast<nanoseconds>(
                    mDescriptor->duration * adpfConfig->mStaleTimeFactor / 2.0));
            setSessionUclampMin(adpfConfig->mUclampMinHigh);
            break;
        case SessionHint::CPU_LOAD_RESUME:
            setSessionUclampMin(mDescriptor->current_min);
            break;
        default:
            ALOGE("Error: hint is invalid");
            return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }
    tryToSendPowerHint(toString(hint));
    mLastUpdatedTime.store(std::chrono::steady_clock::now());
    if (ATRACE_ENABLED()) {
        mLastHintSent = static_cast<int>(hint);
        traceSessionVal("session_hint", static_cast<int>(hint));
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus PowerHintSession::setThreads(const std::vector<int32_t> &threadIds) {
    if (mSessionClosed) {
        ALOGE("Error: session is dead");
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    if (threadIds.size() == 0) {
        LOG(ERROR) << "Error: threadIds.size() shouldn't be " << threadIds.size();
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    PowerSessionManager::getInstance()->removeThreadsFromPowerSession(this);
    mDescriptor->threadIds.resize(threadIds.size());
    std::copy(threadIds.begin(), threadIds.end(), back_inserter(mDescriptor->threadIds));
    PowerSessionManager::getInstance()->addThreadsFromPowerSession(this);
    // init boost
    setSessionUclampMin(HintManager::GetInstance()->GetAdpfProfile()->mUclampMinInit);
    return ndk::ScopedAStatus::ok();
}

std::string AppHintDesc::toString() const {
    std::string out =
            StringPrintf("session %" PRIxPTR "\n", reinterpret_cast<uintptr_t>(this) & 0xffff);
    const int64_t durationNanos = duration.count();
    out.append(StringPrintf("  duration: %" PRId64 " ns\n", durationNanos));
    out.append(StringPrintf("  uclamp.min: %d \n", current_min));
    out.append(StringPrintf("  uid: %d, tgid: %d\n", uid, tgid));

    out.append("  threadIds: [");
    bool first = true;
    for (int tid : threadIds) {
        if (!first) {
            out.append(", ");
        }
        out.append(std::to_string(tid));
        first = false;
    }
    out.append("]\n");
    return out;
}

bool PowerHintSession::isActive() {
    return mDescriptor->is_active.load();
}

bool PowerHintSession::isTimeout() {
    auto now = std::chrono::steady_clock::now();
    time_point<steady_clock> staleTime =
            mLastUpdatedTime.load() +
            nanoseconds(static_cast<int64_t>(
                    mDescriptor->duration.count() *
                    HintManager::GetInstance()->GetAdpfProfile()->mStaleTimeFactor));
    return now >= staleTime;
}

const std::vector<int32_t> &PowerHintSession::getTidList() const {
    return mDescriptor->threadIds;
}

bool PowerHintSession::disableTemporaryBoost() {
    if (ATRACE_ENABLED()) {
        if (mLastHintSent != -1) {
            mLastHintSent = -1;
            traceSessionVal("session_hint", -1);
        }
    }

    // replace temporary uclamp_min value with true min
    std::optional<int> trueMin = mNextUclampMin.load();
    if (trueMin.has_value()) {
        std::lock_guard<std::mutex> guard(mSessionLock);
        mDescriptor->current_min = *trueMin;
        mNextUclampMin.store(std::nullopt);
        return true;
    }

    return false;
}

void PowerHintSession::setStale() {
    // Make sure any temporary boost is disabled
    disableTemporaryBoost();
    // Reset to default uclamp value.
    PowerSessionManager::getInstance()->setUclampMin(this, 0);
    // Deliver a task to check if all sessions are inactive.
    updateUniveralBoostMode();
    if (ATRACE_ENABLED()) {
        traceSessionVal("min", 0);
    }
}

void PowerHintSession::SessionTimerHandler::updateTimer(nanoseconds delay) {
    mTimeout.store(steady_clock::now() + delay);
    {
        std::lock_guard<std::mutex> guard(mMessageLock);
        sp<MessageHandler> selfPtr = sp<MessageHandler>::fromExisting(this);
        PowerHintMonitor::getInstance()->getLooper()->removeMessages(selfPtr);
        PowerHintMonitor::getInstance()->getLooper()->sendMessageDelayed(delay.count(), selfPtr,
                                                                         NULL);
    }
    if (ATRACE_ENABLED()) {
        mSession->traceSessionVal(("timer." + mName).c_str(), 0);
    }
}

void PowerHintSession::SessionTimerHandler::handleMessage(const Message &) {
    std::lock_guard<std::mutex> guard(mClosedLock);
    if (mIsSessionDead) {
        return;
    }
    time_point now = steady_clock::now();
    int64_t next = (mTimeout.load() - now).count();
    if (next > 0) {
        // Schedule for the stale timeout check.
        std::lock_guard<std::mutex> guard(mMessageLock);
        sp<MessageHandler> selfPtr = sp<MessageHandler>::fromExisting(this);
        PowerHintMonitor::getInstance()->getLooper()->removeMessages(selfPtr);
        PowerHintMonitor::getInstance()->getLooper()->sendMessageDelayed(next, selfPtr, NULL);
    } else {
        onTimeout();
    }
    if (ATRACE_ENABLED()) {
        mSession->traceSessionVal(("timer." + mName).c_str(), next > 0 ? 0 : 1);
    }
}

void PowerHintSession::SessionTimerHandler::setSessionDead() {
    std::lock_guard<std::mutex> guard(mClosedLock);
    mIsSessionDead = true;
    PowerHintMonitor::getInstance()->getLooper()->removeMessages(
            sp<MessageHandler>::fromExisting(this));
}

void PowerHintSession::StaleTimerHandler::updateTimer() {
    SessionTimerHandler::updateTimer(duration_cast<nanoseconds>(
            mSession->mDescriptor->duration *
            HintManager::GetInstance()->GetAdpfProfile()->mStaleTimeFactor));
}

void PowerHintSession::StaleTimerHandler::onTimeout() {
    mSession->setStale();
}

void PowerHintSession::BoostTimerHandler::onTimeout() {
    if (mSession->disableTemporaryBoost()) {
        mSession->setSessionUclampMin(mSession->getUclampMin(), false);
    }
}

}  // namespace pixel
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace google
}  // namespace aidl
