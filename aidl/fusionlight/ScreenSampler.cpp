/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ScreenSampler.h"

#include <array>

#include <aidl/vendor/lineage/oplus_fusionlight/ScreenSample.h>
#include <android-base/logging.h>
#include <android/gui/BnScreenCaptureListener.h>
#include <gui/AidlUtil.h>
#include <gui/SurfaceComposerClient.h>
#include <ui/DisplayState.h>
#include <utils/SystemClock.h>

using namespace std::chrono_literals;

namespace aidl::vendor::lineage::oplus_fusionlight {
namespace {

constexpr auto kCaptureCompletionTimeout = 500ms;
constexpr auto kFenceTimeout = 250ms;
constexpr uint32_t kScreenshotSampleStride = 3;

// Extracted from the stock OplusSurfaceFlinger::calcPixelsRGB implementation.
constexpr std::array<float, 256> kScreenshotColorLut = {
        0.00f,   0.00f,   0.00f,   0.00f,   0.00f,   0.02f,   0.04f,   0.04f,
        0.26f,   0.49f,   0.71f,   0.94f,   1.17f,   1.40f,   1.64f,   1.87f,
        2.12f,   2.36f,   2.61f,   2.86f,   3.12f,   3.37f,   3.63f,   3.90f,
        4.17f,   4.44f,   4.71f,   4.99f,   5.27f,   5.56f,   5.85f,   6.14f,
        6.44f,   6.74f,   7.04f,   7.35f,   7.67f,   7.98f,   8.30f,   8.63f,
        8.96f,   9.29f,   9.63f,   9.97f,  10.31f,  10.66f,  11.02f,  11.38f,
       11.74f,  12.11f,  12.48f,  12.86f,  13.24f,  13.63f,  14.02f,  14.42f,
       14.82f,  15.23f,  15.64f,  16.06f,  16.48f,  16.91f,  17.34f,  17.78f,
       18.22f,  18.67f,  19.12f,  19.58f,  20.04f,  20.51f,  20.99f,  21.47f,
       21.96f,  22.45f,  22.95f,  23.45f,  23.96f,  24.48f,  25.00f,  25.52f,
       26.06f,  26.60f,  27.14f,  27.70f,  28.25f,  28.82f,  29.39f,  29.97f,
       30.55f,  31.14f,  31.74f,  32.34f,  32.95f,  33.57f,  34.19f,  34.82f,
       35.46f,  36.10f,  36.75f,  37.41f,  38.07f,  38.74f,  39.42f,  40.10f,
       40.80f,  41.50f,  42.20f,  42.92f,  43.64f,  44.37f,  45.11f,  45.85f,
       46.60f,  47.36f,  48.13f,  48.90f,  49.68f,  50.47f,  51.27f,  52.08f,
       52.89f,  53.71f,  54.54f,  55.38f,  56.22f,  57.08f,  57.94f,  58.81f,
       59.69f,  60.57f,  61.47f,  62.37f,  63.28f,  64.20f,  65.13f,  66.07f,
       67.02f,  67.97f,  68.94f,  69.91f,  70.89f,  71.88f,  72.88f,  73.89f,
       74.90f,  75.93f,  76.97f,  78.01f,  79.06f,  80.13f,  81.20f,  82.28f,
       83.37f,  84.47f,  85.58f,  86.70f,  87.83f,  88.97f,  90.12f,  91.28f,
       92.45f,  93.62f,  94.81f,  96.01f,  97.22f,  98.43f,  99.66f, 100.90f,
      102.15f, 103.41f, 104.67f, 105.95f, 107.24f, 108.54f, 109.85f, 111.17f,
      112.50f, 113.85f, 115.20f, 116.56f, 117.93f, 119.32f, 120.71f, 122.12f,
      123.54f, 124.96f, 126.40f, 127.85f, 129.31f, 130.79f, 132.27f, 133.76f,
      135.27f, 136.79f, 138.31f, 139.85f, 141.41f, 142.97f, 144.54f, 146.13f,
      147.73f, 149.34f, 150.96f, 152.59f, 154.24f, 155.89f, 157.56f, 159.24f,
      160.93f, 162.64f, 164.36f, 166.08f, 167.83f, 169.58f, 171.35f, 173.12f,
      174.91f, 176.72f, 178.53f, 180.36f, 182.20f, 184.05f, 185.92f, 187.80f,
      189.69f, 191.59f, 193.51f, 195.44f, 197.39f, 199.34f, 201.31f, 203.29f,
      205.29f, 207.30f, 209.32f, 211.35f, 213.40f, 215.46f, 217.54f, 219.63f,
      221.73f, 223.85f, 225.98f, 228.12f, 230.28f, 232.45f, 234.63f, 236.83f,
      239.04f, 241.27f, 243.51f, 245.76f, 248.03f, 250.31f, 252.61f, 255.00f,
};

float FindNearestColorCode(float lut_sum, uint32_t sample_count) {
    const uint32_t target = static_cast<uint32_t>(lut_sum / sample_count * 100.0f);
    uint32_t best_difference = 100'000;
    int32_t best_code = 0;
    for (int32_t code = 0; code < 256; ++code) {
        const uint32_t value =
                static_cast<uint32_t>(kScreenshotColorLut[code] * 100.0f);
        const uint32_t difference = value >= target ? value - target : target - value;
        if (difference < best_difference) {
            best_difference = difference;
            best_code = code;
        }
    }
    return best_code;
}

class TimedScreenCaptureListener final : public ::android::gui::BnScreenCaptureListener {
  public:
    ::android::binder::Status onScreenCaptureCompleted(
            const ::android::gui::ScreenCaptureResults& results) override {
        {
            std::lock_guard lock(mutex_);
            results_ = results;
            completed_ = true;
        }
        condition_.notify_one();
        return ::android::binder::Status::ok();
    }

    bool waitForResults(std::chrono::milliseconds timeout,
                        ::android::gui::ScreenCaptureResults& results) {
        std::unique_lock lock(mutex_);
        if (!condition_.wait_for(lock, timeout, [this] { return completed_; })) {
            return false;
        }
        results = std::move(results_);
        return true;
    }

  private:
    std::mutex mutex_;
    std::condition_variable condition_;
    ::android::gui::ScreenCaptureResults results_;
    bool completed_ = false;
};

struct DisplayTarget {
    ::android::sp<::android::IBinder> token;
    ::android::ui::DisplayState state;
};

std::optional<DisplayTarget> GetInternalDisplay() {
    const auto display_ids = ::android::SurfaceComposerClient::getPhysicalDisplayIds();
    if (display_ids.empty()) {
        return std::nullopt;
    }

    auto selected_id = display_ids.front();
    for (const auto display_id : display_ids) {
        ::android::ui::StaticDisplayInfo info;
        if (::android::SurfaceComposerClient::getStaticDisplayInfo(display_id.value, &info) ==
                    ::android::NO_ERROR &&
            info.connectionType == ::android::ui::DisplayConnectionType::Internal) {
            selected_id = display_id;
            break;
        }
    }

    DisplayTarget target;
    target.token = ::android::SurfaceComposerClient::getPhysicalDisplayToken(selected_id);
    if (target.token == nullptr ||
        ::android::SurfaceComposerClient::getDisplayState(target.token, &target.state) !=
                ::android::NO_ERROR ||
        !target.state.layerStackSpaceRect.isValid() || target.state.layerStackSpaceRect.isEmpty()) {
        return std::nullopt;
    }
    return target;
}

int32_t ScaleFloor(int32_t value, int32_t extent, int32_t reference_extent) {
    return static_cast<int32_t>(static_cast<int64_t>(value) * extent / reference_extent);
}

int32_t ScaleCeil(int32_t value, int32_t extent, int32_t reference_extent) {
    return static_cast<int32_t>((static_cast<int64_t>(value) * extent + reference_extent - 1) /
                                reference_extent);
}

std::optional<::android::Rect> MapCropToDisplay(const SamplingConfig& config,
                                                const ::android::ui::DisplayState& display_state) {
    const int32_t logical_width = display_state.layerStackSpaceRect.getWidth();
    const int32_t logical_height = display_state.layerStackSpaceRect.getHeight();
    const bool quarter_turn = display_state.orientation == ::android::ui::ROTATION_90 ||
                              display_state.orientation == ::android::ui::ROTATION_270;
    const int32_t natural_width = quarter_turn ? logical_height : logical_width;
    const int32_t natural_height = quarter_turn ? logical_width : logical_height;

    const ::android::Rect natural_crop{
            ScaleFloor(config.left, natural_width, config.referenceWidth),
            ScaleFloor(config.top, natural_height, config.referenceHeight),
            ScaleCeil(config.right, natural_width, config.referenceWidth),
            ScaleCeil(config.bottom, natural_height, config.referenceHeight),
    };

    ::android::Rect crop;
    switch (display_state.orientation) {
        case ::android::ui::ROTATION_90:
            crop = ::android::Rect{natural_height - natural_crop.bottom, natural_crop.left,
                                   natural_height - natural_crop.top, natural_crop.right};
            break;
        case ::android::ui::ROTATION_180:
            crop = ::android::Rect{
                    natural_width - natural_crop.right, natural_height - natural_crop.bottom,
                    natural_width - natural_crop.left, natural_height - natural_crop.top};
            break;
        case ::android::ui::ROTATION_270:
            crop = ::android::Rect{natural_crop.top, natural_width - natural_crop.right,
                                   natural_crop.bottom, natural_width - natural_crop.left};
            break;
        case ::android::ui::ROTATION_0:
        default:
            crop = natural_crop;
            break;
    }

    crop.left = std::clamp(crop.left, 0, logical_width);
    crop.top = std::clamp(crop.top, 0, logical_height);
    crop.right = std::clamp(crop.right, 0, logical_width);
    crop.bottom = std::clamp(crop.bottom, 0, logical_height);
    if (crop.isEmpty()) {
        return std::nullopt;
    }
    return crop;
}

}  // namespace

struct ScreenSampler::CapturedFrame {
    int64_t frame_start_ns = 0;
    int64_t frame_end_ns = 0;
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
};

struct ScreenSampler::DeathCookie {
    std::weak_ptr<ScreenSampler> service;
    int64_t session_id;
};

struct ScreenSampler::Session {
    Session(int64_t id, const SamplingConfig& sampling_config,
            std::shared_ptr<IScreenSampleCallback> sample_callback)
        : session_id(id),
          config(sampling_config),
          callback(std::move(sample_callback)),
          next_sample_at(std::chrono::steady_clock::now()) {}

    const int64_t session_id;
    const SamplingConfig config;
    const std::shared_ptr<IScreenSampleCallback> callback;
    std::chrono::steady_clock::time_point next_sample_at;
    std::atomic_bool active = true;
    std::mutex dispatch_mutex;
    std::condition_variable dispatch_condition;
    bool dispatch_in_flight = false;
    std::thread::id dispatch_thread;
    DeathCookie* death_cookie = nullptr;
    std::atomic_bool death_linked = false;
};

ScreenSampler::ScreenSampler()
    : death_recipient_(AIBinder_DeathRecipient_new(onCallbackDied)),
      worker_(&ScreenSampler::workerLoop, this) {
    AIBinder_DeathRecipient_setOnUnlinked(death_recipient_.get(), onCallbackUnlinked);
}

ScreenSampler::~ScreenSampler() {
    std::vector<std::shared_ptr<Session>> sessions;
    {
        std::lock_guard lock(mutex_);
        shutting_down_ = true;
        sessions.reserve(sessions_.size());
        for (auto& entry : sessions_) {
            auto& session = entry.second;
            session->active = false;
            sessions.push_back(session);
        }
        sessions_.clear();
    }
    condition_.notify_all();
    worker_.join();

    for (const auto& session : sessions) {
        unlinkCallback(session);
    }
}

ndk::ScopedAStatus ScreenSampler::startSampling(
        const SamplingConfig& config, const std::shared_ptr<IScreenSampleCallback>& callback,
        int64_t* aidl_return) {
    if (aidl_return == nullptr || callback == nullptr || !isValidConfig(config)) {
        return ndk::ScopedAStatus::fromExceptionCodeWithMessage(
                EX_ILLEGAL_ARGUMENT, "Invalid sampling configuration or callback");
    }

    std::shared_ptr<Session> session;
    binder_status_t link_status = STATUS_OK;
    {
        std::lock_guard lock(mutex_);
        if (shutting_down_) {
            return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
        }

        while (next_session_id_ <= 0 || sessions_.find(next_session_id_) != sessions_.end()) {
            next_session_id_ = next_session_id_ == std::numeric_limits<int64_t>::max()
                                       ? 1
                                       : next_session_id_ + 1;
        }
        const int64_t session_id = next_session_id_;
        next_session_id_ =
                next_session_id_ == std::numeric_limits<int64_t>::max() ? 1 : next_session_id_ + 1;
        session = std::make_shared<Session>(session_id, config, callback);
        sessions_.emplace(session_id, session);

        const ndk::SpAIBinder callback_binder = callback->asBinder();
        if (callback_binder.get() != nullptr && AIBinder_isRemote(callback_binder.get())) {
            session->death_cookie = new DeathCookie{ref<ScreenSampler>(), session_id};
            link_status = AIBinder_linkToDeath(callback_binder.get(), death_recipient_.get(),
                                               session->death_cookie);
            if (link_status == STATUS_OK) {
                session->death_linked = true;
            } else {
                session->death_cookie = nullptr;
                session->active = false;
                sessions_.erase(session_id);
            }
        }

        if (link_status == STATUS_OK) {
            *aidl_return = session_id;
        }
    }

    if (link_status != STATUS_OK) {
        return ndk::ScopedAStatus::fromStatus(link_status);
    }

    LOG(INFO) << "Started FusionLight screen sampling session " << session->session_id;
    condition_.notify_one();
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus ScreenSampler::stopSampling(int64_t session_id) {
    std::shared_ptr<Session> session;
    {
        std::lock_guard lock(mutex_);
        const auto session_it = sessions_.find(session_id);
        if (session_it == sessions_.end()) {
            return ndk::ScopedAStatus::ok();
        }
        session = session_it->second;
        session->active = false;
        sessions_.erase(session_it);
    }
    condition_.notify_one();

    {
        std::unique_lock dispatch_lock(session->dispatch_mutex);
        if (session->dispatch_in_flight && session->dispatch_thread != std::this_thread::get_id()) {
            session->dispatch_condition.wait(dispatch_lock,
                                             [&session] { return !session->dispatch_in_flight; });
        }
    }
    unlinkCallback(session);
    LOG(INFO) << "Stopped FusionLight screen sampling session " << session_id;
    return ndk::ScopedAStatus::ok();
}

bool ScreenSampler::isValidConfig(const SamplingConfig& config) {
    return config.referenceWidth > 0 && config.referenceHeight > 0 && config.left >= 0 &&
           config.top >= 0 && config.right > config.left && config.bottom > config.top &&
           config.right <= config.referenceWidth && config.bottom <= config.referenceHeight &&
           config.samplingPeriodMillis > 0;
}

bool ScreenSampler::captureFrame(const SamplingConfig& config, CapturedFrame& frame) const {
    const auto display = GetInternalDisplay();
    if (!display.has_value()) {
        return false;
    }

    const auto crop = MapCropToDisplay(config, display->state);
    if (!crop.has_value()) {
        return false;
    }

    ::android::DisplayCaptureArgs args;
    args.captureArgs.pixelFormat = ::android::PIXEL_FORMAT_RGBA_8888;
    args.captureArgs.dataspace =
            static_cast<int32_t>(::android::ui::Dataspace::V0_SRGB);
    args.captureArgs.sourceCrop = ::android::gui::aidl_utils::toARect(*crop);
    args.captureArgs.secureLayerMode = ::android::gui::SecureLayerMode::Capture;
    args.captureArgs.preserveDisplayColors = false;
    args.displayToken = display->token;
    args.width = crop->getWidth();
    args.height = crop->getHeight();

    frame.frame_start_ns = ::android::elapsedRealtimeNano();
    const ::android::sp<TimedScreenCaptureListener> listener =
            ::android::sp<TimedScreenCaptureListener>::make();
    if (::android::ScreenshotClient::captureDisplay(args, listener) != ::android::NO_ERROR) {
        return false;
    }

    ::android::gui::ScreenCaptureResults results;
    if (!listener->waitForResults(kCaptureCompletionTimeout, results)) {
        return false;
    }
    if (results.capturedDataspace != ::android::ui::Dataspace::V0_SRGB) {
        LOG(WARNING) << "SurfaceFlinger returned unexpected screenshot dataspace "
                     << static_cast<int32_t>(results.capturedDataspace);
        return false;
    }
    if (!results.fenceResult.ok()) {
        return false;
    }

    const ::android::sp<::android::Fence>& fence = results.fenceResult.value();
    if (fence != nullptr) {
        const ::android::status_t fence_status = fence->wait(kFenceTimeout.count());
        if (fence_status != ::android::NO_ERROR) {
            return false;
        }
    }
    frame.frame_end_ns = ::android::elapsedRealtimeNano();

    const ::android::sp<::android::GraphicBuffer>& buffer = results.buffer;
    if (buffer == nullptr || buffer->getPixelFormat() != ::android::PIXEL_FORMAT_RGBA_8888 ||
        buffer->getWidth() == 0 || buffer->getHeight() == 0 ||
        buffer->getStride() < buffer->getWidth()) {
        return false;
    }

    void* raw_pixels = nullptr;
    if (buffer->lock(::android::GraphicBuffer::USAGE_SW_READ_OFTEN, &raw_pixels) !=
                ::android::NO_ERROR ||
        raw_pixels == nullptr) {
        return false;
    }

    const auto* pixels = static_cast<const uint8_t*>(raw_pixels);
    const uint32_t width = buffer->getWidth();
    const uint32_t height = buffer->getHeight();
    const uint32_t stride = buffer->getStride();
    float red_sum = 0.0f;
    float green_sum = 0.0f;
    float blue_sum = 0.0f;
    uint32_t sample_count = 0;
    for (uint32_t y = 0; y < height; y += kScreenshotSampleStride) {
        const uint8_t* row = pixels + static_cast<size_t>(y) * stride * 4;
        for (uint32_t x = 0; x < width; x += kScreenshotSampleStride) {
            red_sum += kScreenshotColorLut[row[x * 4]];
            green_sum += kScreenshotColorLut[row[x * 4 + 1]];
            blue_sum += kScreenshotColorLut[row[x * 4 + 2]];
            ++sample_count;
        }
    }

    const ::android::status_t unlock_status = buffer->unlock();
    if (unlock_status != ::android::NO_ERROR) {
        return false;
    }

    frame.r = FindNearestColorCode(red_sum, sample_count);
    frame.g = FindNearestColorCode(green_sum, sample_count);
    frame.b = FindNearestColorCode(blue_sum, sample_count);
    return true;
}

void ScreenSampler::dispatchSample(const std::shared_ptr<Session>& session, CapturedFrame frame) {
    {
        std::lock_guard dispatch_lock(session->dispatch_mutex);
        if (!session->active) {
            return;
        }
        session->dispatch_in_flight = true;
        session->dispatch_thread = std::this_thread::get_id();
    }

    ScreenSample sample;
    bool should_dispatch = false;
    {
        std::lock_guard lock(mutex_);
        const auto session_it = sessions_.find(session->session_id);
        if (session_it != sessions_.end() && session_it->second == session && session->active) {
            sample.sessionId = session->session_id;
            sample.frameStartNs = frame.frame_start_ns;
            sample.frameEndNs = frame.frame_end_ns;
            sample.r = frame.r;
            sample.g = frame.g;
            sample.b = frame.b;
            should_dispatch = true;
        }
    }

    ndk::ScopedAStatus callback_status = ndk::ScopedAStatus::ok();
    if (should_dispatch) {
        callback_status = session->callback->onSample(sample);
    }

    {
        std::lock_guard dispatch_lock(session->dispatch_mutex);
        session->dispatch_in_flight = false;
        session->dispatch_thread = {};
    }
    session->dispatch_condition.notify_all();

    if (should_dispatch && !callback_status.isOk()) {
        removeSession(session, true);
    }
}

void ScreenSampler::workerLoop() {
    std::unique_lock lock(mutex_);
    while (!shutting_down_) {
        if (sessions_.empty()) {
            condition_.wait(lock, [this] { return shutting_down_ || !sessions_.empty(); });
            continue;
        }

        const auto next_session = std::min_element(
                sessions_.begin(), sessions_.end(), [](const auto& left, const auto& right) {
                    return left.second->next_sample_at < right.second->next_sample_at;
                });
        const auto now = std::chrono::steady_clock::now();
        if (next_session->second->next_sample_at > now) {
            condition_.wait_until(lock, next_session->second->next_sample_at);
            continue;
        }

        const std::shared_ptr<Session> session = next_session->second;
        session->next_sample_at =
                now + std::chrono::milliseconds(session->config.samplingPeriodMillis);

        lock.unlock();
        CapturedFrame frame;
        if (session->active && captureFrame(session->config, frame)) {
            dispatchSample(session, std::move(frame));
        }
        lock.lock();

        const auto session_it = sessions_.find(session->session_id);
        const auto after_capture = std::chrono::steady_clock::now();
        if (session_it != sessions_.end() && session_it->second == session &&
            session->next_sample_at <= after_capture) {
            session->next_sample_at =
                    after_capture + std::chrono::milliseconds(session->config.samplingPeriodMillis);
        }
    }
}

void ScreenSampler::handleCallbackDeath(int64_t session_id) {
    std::shared_ptr<Session> session;
    {
        std::lock_guard lock(mutex_);
        const auto session_it = sessions_.find(session_id);
        if (session_it == sessions_.end()) {
            return;
        }
        session = session_it->second;
        session->active = false;
        session->death_linked = false;
        sessions_.erase(session_it);
    }
    condition_.notify_one();
    LOG(INFO) << "Removed screen sampling session " << session_id << " after callback death";
}

void ScreenSampler::removeSession(const std::shared_ptr<Session>& session, bool unlink_callback) {
    {
        std::lock_guard lock(mutex_);
        session->active = false;
        const auto session_it = sessions_.find(session->session_id);
        if (session_it != sessions_.end() && session_it->second == session) {
            sessions_.erase(session_it);
        }
    }
    condition_.notify_one();
    if (unlink_callback) {
        unlinkCallback(session);
    }
}

void ScreenSampler::unlinkCallback(const std::shared_ptr<Session>& session) {
    if (!session->death_linked.exchange(false)) {
        return;
    }
    const ndk::SpAIBinder callback_binder = session->callback->asBinder();
    if (callback_binder.get() != nullptr) {
        AIBinder_unlinkToDeath(callback_binder.get(), death_recipient_.get(),
                               session->death_cookie);
    }
}

void ScreenSampler::onCallbackDied(void* cookie) {
    const auto* death_cookie = static_cast<DeathCookie*>(cookie);
    if (const auto service = death_cookie->service.lock()) {
        service->handleCallbackDeath(death_cookie->session_id);
    }
}

void ScreenSampler::onCallbackUnlinked(void* cookie) {
    delete static_cast<DeathCookie*>(cookie);
}

}  // namespace aidl::vendor::lineage::oplus_fusionlight
