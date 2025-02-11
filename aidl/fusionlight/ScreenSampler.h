/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <thread>

#include <aidl/vendor/lineage/oplus_fusionlight/BnScreenSampler.h>

namespace aidl::vendor::lineage::oplus_fusionlight {

class ScreenSampler final : public BnScreenSampler {
  public:
    ScreenSampler();
    ~ScreenSampler() override;

    ndk::ScopedAStatus startSampling(const SamplingConfig& config,
                                     const std::shared_ptr<IScreenSampleCallback>& callback,
                                     int64_t* aidl_return) override;
    ndk::ScopedAStatus stopSampling(int64_t session_id) override;

  private:
    struct CapturedFrame;
    struct DeathCookie;
    struct Session;

    static void onCallbackDied(void* cookie);
    static void onCallbackUnlinked(void* cookie);
    static bool isValidConfig(const SamplingConfig& config);

    bool captureFrame(const SamplingConfig& config, CapturedFrame& frame) const;
    void dispatchSample(const std::shared_ptr<Session>& session, CapturedFrame frame);
    void handleCallbackDeath(int64_t session_id);
    void removeSession(const std::shared_ptr<Session>& session, bool unlink_callback);
    void unlinkCallback(const std::shared_ptr<Session>& session);
    void workerLoop();

    ndk::ScopedAIBinder_DeathRecipient death_recipient_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::unordered_map<int64_t, std::shared_ptr<Session>> sessions_;
    int64_t next_session_id_ = 1;
    bool shutting_down_ = false;
    std::thread worker_;
};

}  // namespace aidl::vendor::lineage::oplus_fusionlight
