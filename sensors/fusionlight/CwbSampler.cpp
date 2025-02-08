/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "CwbSampler.h"

#include <aidl/vendor/qti/hardware/display/config/Attributes.h>
#include <aidl/vendor/qti/hardware/display/config/BnDisplayConfigCallback.h>
#include <aidl/vendor/qti/hardware/display/config/IDisplayConfig.h>
#include <aidl/vendor/qti/hardware/display/config/Rect.h>
#include <aidlcommonsupport/NativeHandle.h>
#include <android-base/logging.h>
#include <hardware/gralloc.h>
#include <ui/GraphicBuffer.h>
#include <ui/PixelFormat.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <ctime>
#include <utility>

namespace android {
namespace hardware {
namespace sensors {
namespace V2_1 {
namespace subhal {
namespace implementation {
namespace fusionlight {
namespace {

using namespace std::chrono_literals;

using aidl::android::hardware::common::NativeHandle;
using aidl::vendor::qti::hardware::display::config::Attributes;
using aidl::vendor::qti::hardware::display::config::BnDisplayConfigCallback;
using aidl::vendor::qti::hardware::display::config::CameraSmoothOp;
using aidl::vendor::qti::hardware::display::config::Concurrency;
using aidl::vendor::qti::hardware::display::config::DisplayType;
using aidl::vendor::qti::hardware::display::config::IDisplayConfig;
using aidl::vendor::qti::hardware::display::config::Rect;
using aidl::vendor::qti::hardware::display::config::TUIEventType;

constexpr auto kServiceRetryPeriod = 2s;
constexpr auto kRequestTimeout = 1s;
constexpr int32_t kPrimaryDisplay = static_cast<int32_t>(DisplayType::PRIMARY);
constexpr uint64_t kBufferUsage = GRALLOC_USAGE_SW_READ_OFTEN | GRALLOC_USAGE_SW_WRITE_OFTEN;

int64_t GetBootTimeNs() {
    timespec time = {};
    if (clock_gettime(CLOCK_BOOTTIME, &time) != 0) {
        return 0;
    }
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
                   std::chrono::seconds(time.tv_sec) + std::chrono::nanoseconds(time.tv_nsec))
            .count();
}

const std::array<double, 256>& GetSrgbToLinearLut() {
    static const std::array<double, 256> lut = [] {
        std::array<double, 256> values;
        for (int value = 0; value < values.size(); ++value) {
            const double srgb = value / 255.0;
            values[value] = srgb <= 0.04045 ? srgb / 12.92 : std::pow((srgb + 0.055) / 1.055, 2.4);
        }
        return values;
    }();
    return lut;
}

double SrgbToLinear(uint8_t value) {
    return GetSrgbToLinearLut()[value];
}

int32_t LinearToSrgb(double value) {
    const uint32_t scaled_value = std::clamp(value, 0.0, 1.0) * 1000.0;
    const auto& lut = GetSrgbToLinearLut();
    int32_t result = 0;
    for (int value_index = 255; value_index >= 0; --value_index) {
        if (scaled_value <= static_cast<uint32_t>(lut[value_index] * 1000.0)) {
            result = value_index;
        }
    }
    return result;
}

Rect ScaleCrop(const FusionConfig& config, int32_t width, int32_t height) {
    Rect crop;
    crop.left = static_cast<int64_t>(config.crop_left) * width / config.reference_width;
    crop.top = static_cast<int64_t>(config.crop_top) * height / config.reference_height;
    crop.right = static_cast<int64_t>(config.crop_right) * width / config.reference_width;
    crop.bottom = static_cast<int64_t>(config.crop_bottom) * height / config.reference_height;
    return crop;
}

std::optional<CwbSample> ReadSample(const sp<GraphicBuffer>& buffer, const Rect& crop,
                                    const FusionConfig& config, int64_t frame_start_ns,
                                    int64_t frame_end_ns) {
    void* base = nullptr;
    const status_t status = buffer->lock(GRALLOC_USAGE_SW_READ_OFTEN, &base);
    if (status != NO_ERROR || base == nullptr) {
        LOG(WARNING) << "Unable to map CWB buffer: " << status;
        return std::nullopt;
    }

    const int32_t crop_width = crop.right - crop.left;
    const int32_t crop_height = crop.bottom - crop.top;
    const bool use_weights =
            config.cwb_screenshot_weighted &&
            config.cwb_weights.size() == static_cast<size_t>(crop_width * crop_height);
    const int32_t step = use_weights ? 1 : 3;
    const auto* pixels = static_cast<const uint8_t*>(base);
    const size_t row_bytes = static_cast<size_t>(buffer->getStride()) * 3;
    double red = 0.0;
    double green = 0.0;
    double blue = 0.0;
    double weight_sum = 0.0;
    for (int32_t y = crop.top; y < crop.bottom; y += step) {
        for (int32_t x = crop.left; x < crop.right; x += step) {
            double weight = 1.0;
            if (use_weights) {
                const int32_t index = (y - crop.top) * crop_width + x - crop.left;
                weight = config.cwb_weights[index];
            }
            const auto* pixel = pixels + static_cast<size_t>(y) * row_bytes + x * 3;
            red += SrgbToLinear(pixel[0]) * weight;
            green += SrgbToLinear(pixel[1]) * weight;
            blue += SrgbToLinear(pixel[2]) * weight;
            weight_sum += weight;
        }
    }
    buffer->unlock();

    if (weight_sum <= 0.0) {
        LOG(WARNING) << "CWB crop has no usable pixels";
        return std::nullopt;
    }
    return CwbSample{
            LinearToSrgb(red / weight_sum),
            LinearToSrgb(green / weight_sum),
            LinearToSrgb(blue / weight_sum),
            frame_start_ns,
            frame_end_ns,
    };
}

}  // namespace

struct CwbSampler::SharedState {
    explicit SharedState(SampleCallback callback) : sample_callback(std::move(callback)) {}

    std::mutex mutex;
    std::condition_variable condition;
    SampleCallback sample_callback;
    FusionConfig config;
    uint64_t active_request = 0;
    uint64_t completed_request = 0;
    int32_t completed_error = 0;
    int64_t completed_time_ns = 0;
    bool active = false;
    bool immediate = false;
};

class CwbSampler::DisplayCallback final : public BnDisplayConfigCallback {
  public:
    DisplayCallback(std::weak_ptr<SharedState> state, uint64_t request)
        : state_(std::move(state)), request_(request) {}

    ndk::ScopedAStatus notifyCWBBufferDone(int32_t error, const NativeHandle&) override {
        const auto state = state_.lock();
        if (state == nullptr) {
            return ndk::ScopedAStatus::ok();
        }
        {
            std::lock_guard lock(state->mutex);
            if (!state->active || state->active_request != request_) {
                return ndk::ScopedAStatus::ok();
            }
            state->completed_request = request_;
            state->completed_error = error;
            state->completed_time_ns = GetBootTimeNs();
        }
        state->condition.notify_all();
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus notifyQsyncChange(bool, int32_t, int32_t) override {
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus notifyIdleStatus(bool) override { return ndk::ScopedAStatus::ok(); }

    ndk::ScopedAStatus notifyCameraSmoothInfo(CameraSmoothOp, int32_t) override {
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus notifyResolutionChange(int32_t, const Attributes&) override {
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus notifyFpsMitigation(int32_t, const Attributes&, Concurrency) override {
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus notifyTUIEventDone(int32_t, DisplayType, TUIEventType) override {
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus notifyContentFps(const std::string&, int32_t) override {
        return ndk::ScopedAStatus::ok();
    }

  private:
    std::weak_ptr<SharedState> state_;
    uint64_t request_;
};

CwbSampler::CwbSampler(SampleCallback sample_callback)
    : state_(std::make_shared<SharedState>(std::move(sample_callback))) {}

CwbSampler::~CwbSampler() {
    stop();
    std::lock_guard lock(state_->mutex);
    state_->sample_callback = nullptr;
}

void CwbSampler::start(const FusionConfig& config) {
    {
        std::lock_guard lock(state_->mutex);
        if (state_->active) {
            return;
        }
        state_->config = config;
        state_->active = true;
        state_->immediate = true;
    }
    thread_ = std::thread(threadLoop, state_);
}

void CwbSampler::stop() {
    {
        std::lock_guard lock(state_->mutex);
        if (!state_->active) {
            return;
        }
        state_->active = false;
        state_->immediate = false;
        ++state_->active_request;
    }
    state_->condition.notify_all();
    if (thread_.joinable()) {
        thread_.join();
    }
}

void CwbSampler::requestSample() {
    {
        std::lock_guard lock(state_->mutex);
        if (!state_->active) {
            return;
        }
        state_->immediate = true;
    }
    state_->condition.notify_all();
}

void CwbSampler::threadLoop(const std::shared_ptr<SharedState>& state) {
    FusionConfig config;
    {
        std::lock_guard lock(state->mutex);
        config = state->config;
    }

    const auto period = std::chrono::milliseconds(config.cwb_screenshot_period_millis);
    auto next_sample = std::chrono::steady_clock::now();
    std::shared_ptr<IDisplayConfig> service;
    sp<GraphicBuffer> buffer;
    Rect crop;

    for (;;) {
        {
            std::unique_lock lock(state->mutex);
            state->condition.wait_until(lock, next_sample,
                                        [&] { return !state->active || state->immediate; });
            if (!state->active) {
                return;
            }
            state->immediate = false;
        }

        if (service == nullptr || service->asBinder().get() == nullptr ||
            !AIBinder_isAlive(service->asBinder().get())) {
            service = GetService<IDisplayConfig>();
            buffer.clear();
        }
        if (service == nullptr) {
            LOG(WARNING) << "IDisplayConfig is unavailable";
            SampleCallback callback;
            {
                std::lock_guard lock(state->mutex);
                callback = state->sample_callback;
            }
            if (callback) {
                callback(std::nullopt);
            }
            next_sample = std::chrono::steady_clock::now() + kServiceRetryPeriod;
            continue;
        }

        if (buffer == nullptr) {
            Attributes attributes;
            const auto status = service->getActiveBuiltinDisplayAttributes(&attributes);
            if (!status.isOk() || attributes.xRes <= 0 || attributes.yRes <= 0) {
                LOG(WARNING) << "Unable to get active display resolution: "
                             << status.getDescription();
                SampleCallback sample_callback;
                {
                    std::lock_guard lock(state->mutex);
                    sample_callback = state->sample_callback;
                }
                if (sample_callback) {
                    sample_callback(std::nullopt);
                }
                next_sample = std::chrono::steady_clock::now() + kServiceRetryPeriod;
                continue;
            }
            const int32_t width = attributes.xRes;
            const int32_t height = attributes.yRes;
            crop = ScaleCrop(config, width, height);
            buffer = sp<GraphicBuffer>::make(width, height, PIXEL_FORMAT_RGB_888, 1, kBufferUsage,
                                             "FusionLightCWB");
            if (buffer->initCheck() != NO_ERROR) {
                LOG(ERROR) << "Unable to allocate FusionLight CWB buffer: " << buffer->initCheck();
                buffer.clear();
                SampleCallback sample_callback;
                {
                    std::lock_guard lock(state->mutex);
                    sample_callback = state->sample_callback;
                }
                if (sample_callback) {
                    sample_callback(std::nullopt);
                }
                next_sample = std::chrono::steady_clock::now() + kServiceRetryPeriod;
                continue;
            }
            const int32_t crop_size = (crop.right - crop.left) * (crop.bottom - crop.top);
            const bool weighted = config.cwb_screenshot_weighted &&
                                  config.cwb_weights.size() == static_cast<size_t>(crop_size);
            LOG(INFO) << "FusionLight CWB geometry=" << width << 'x' << height << " crop=["
                      << crop.left << ',' << crop.top << ',' << crop.right << ',' << crop.bottom
                      << "] weighted=" << weighted;
            if (config.cwb_screenshot_weighted && !weighted) {
                LOG(WARNING) << "CWB crop size changed from the profile; using unweighted "
                                "sampling";
            }
        }

        uint64_t request;
        {
            std::lock_guard lock(state->mutex);
            request = ++state->active_request;
        }
        const int64_t frame_start_ns = GetBootTimeNs();
        const auto callback = ndk::SharedRefBase::make<DisplayCallback>(state, request);
        const auto request_status = service->setCWBOutputBuffer(
                callback, kPrimaryDisplay, crop, true, ::android::dupToAidl(buffer->handle));
        if (!request_status.isOk()) {
            LOG(WARNING) << "IDisplayConfig::setCWBOutputBuffer failed: "
                         << request_status.getDescription();
            service.reset();
            buffer.clear();
            SampleCallback sample_callback;
            {
                std::lock_guard lock(state->mutex);
                sample_callback = state->sample_callback;
            }
            if (sample_callback) {
                sample_callback(std::nullopt);
            }
            next_sample = std::chrono::steady_clock::now() + kServiceRetryPeriod;
            continue;
        }

        int32_t error = 0;
        int64_t frame_end_ns = 0;
        bool completed = false;
        {
            std::unique_lock lock(state->mutex);
            completed = state->condition.wait_for(lock, kRequestTimeout, [&] {
                return !state->active || state->completed_request == request;
            });
            if (!state->active) {
                return;
            }
            if (completed) {
                error = state->completed_error;
                frame_end_ns = state->completed_time_ns;
            } else {
                ++state->active_request;
            }
        }

        std::optional<CwbSample> sample;
        if (!completed) {
            LOG(WARNING) << "Timed out waiting for CWB completion";
            buffer.clear();
        } else if (error != 0) {
            LOG(WARNING) << "CWB capture failed with error " << error;
        } else {
            sample = ReadSample(buffer, crop, config, frame_start_ns, frame_end_ns);
        }

        SampleCallback sample_callback;
        {
            std::lock_guard lock(state->mutex);
            sample_callback = state->sample_callback;
        }
        if (sample_callback) {
            sample_callback(std::move(sample));
        }
        next_sample += period;
        if (next_sample < std::chrono::steady_clock::now()) {
            next_sample = std::chrono::steady_clock::now();
        }
    }
}

}  // namespace fusionlight
}  // namespace implementation
}  // namespace subhal
}  // namespace V2_1
}  // namespace sensors
}  // namespace hardware
}  // namespace android
