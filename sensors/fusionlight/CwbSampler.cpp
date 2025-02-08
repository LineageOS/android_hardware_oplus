/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "CwbSampler.h"

#include <aidl/vendor/qti/hardware/display/config/BnDisplayConfigCallback.h>
#include <aidl/vendor/qti/hardware/display/config/IDisplayConfig.h>
#include <aidlcommonsupport/NativeHandle.h>
#include <android-base/logging.h>
#include <ui/ColorSpace.h>
#include <ui/GraphicBuffer.h>

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

std::chrono::nanoseconds GetBootTime() {
    timespec time;
    if (clock_gettime(CLOCK_BOOTTIME, &time) != 0) {
        return 0ns;
    }
    return std::chrono::seconds(time.tv_sec) + std::chrono::nanoseconds(time.tv_nsec);
}

const std::array<float, 256>& GetSrgbToLinearLut() {
    static const std::array<float, 256> lut = [] {
        std::array<float, 256> values;
        const auto decode = ColorSpace::sRGB().getEOTF();
        for (int value = 0; value < values.size(); ++value) {
            values[value] = decode(value / 255.0f);
        }
        return values;
    }();
    return lut;
}

float SrgbToLinear(uint8_t value) {
    return GetSrgbToLinearLut()[value];
}

int32_t LinearToSrgb(float value) {
    const auto& lut = GetSrgbToLinearLut();
    const auto result = std::lower_bound(lut.begin(), lut.end(), std::clamp(value, 0.0f, 1.0f));
    return result == lut.end() ? 255 : result - lut.begin();
}

Rect ScaleCrop(const CwbConfig& config, int32_t width, int32_t height) {
    Rect crop;
    crop.left = config.crop_left * width / config.reference_width;
    crop.top = config.crop_top * height / config.reference_height;
    crop.right = config.crop_right * width / config.reference_width;
    crop.bottom = config.crop_bottom * height / config.reference_height;
    return crop;
}

std::optional<CwbSample> ReadSample(const sp<GraphicBuffer>& buffer, const Rect& crop,
                                    const CwbConfig& config, std::chrono::nanoseconds frame_start,
                                    std::chrono::nanoseconds frame_end) {
    void* base = nullptr;
    const status_t status = buffer->lock(GRALLOC_USAGE_SW_READ_OFTEN, &base);
    if (status != NO_ERROR || base == nullptr) {
        LOG(WARNING) << "Unable to map CWB buffer: " << status;
        return std::nullopt;
    }

    const int32_t crop_width = crop.right - crop.left;
    const int32_t crop_height = crop.bottom - crop.top;
    const bool use_weights =
            config.screenshot_weighted && config.weights.size() == crop_width * crop_height;
    const int32_t step = use_weights ? 1 : 3;
    const auto* pixels = static_cast<const uint8_t*>(base);
    const size_t row_bytes = static_cast<size_t>(buffer->getStride()) * 3;
    float red = 0.0f;
    float green = 0.0f;
    float blue = 0.0f;
    float weight_sum = 0.0f;
    for (int32_t y = crop.top; y < crop.bottom; y += step) {
        for (int32_t x = crop.left; x < crop.right; x += step) {
            float weight = 1.0f;
            if (use_weights) {
                const int32_t index = (y - crop.top) * crop_width + x - crop.left;
                weight = config.weights[index];
            }
            const size_t offset = y * row_bytes + x * 3;
            red += SrgbToLinear(pixels[offset]) * weight;
            green += SrgbToLinear(pixels[offset + 1]) * weight;
            blue += SrgbToLinear(pixels[offset + 2]) * weight;
            weight_sum += weight;
        }
    }
    buffer->unlock();

    if (weight_sum <= 0.0f) {
        LOG(WARNING) << "CWB crop has no usable pixels";
        return std::nullopt;
    }
    return CwbSample{
            LinearToSrgb(red / weight_sum),
            LinearToSrgb(green / weight_sum),
            LinearToSrgb(blue / weight_sum),
            frame_start,
            frame_end,
    };
}

}  // namespace

struct CwbSampler::SharedState {
    explicit SharedState(SampleCallback callback) : sample_callback(std::move(callback)) {}

    std::mutex mutex;
    std::condition_variable condition;
    const SampleCallback sample_callback;
    uint64_t active_request = 0;
    uint64_t completed_request = 0;
    int32_t completed_error = 0;
    std::chrono::nanoseconds completed_time{0};
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
            state->completed_time = GetBootTime();
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
}

void CwbSampler::setConfig(CwbConfig config) {
    config_ = std::move(config);
}

void CwbSampler::start() {
    {
        std::lock_guard lock(state_->mutex);
        if (state_->active) {
            return;
        }
        state_->active = true;
        state_->immediate = true;
    }
    thread_ = std::thread(&CwbSampler::threadLoop, this);
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

void CwbSampler::threadLoop() {
    const auto& state = state_;
    const auto& config = config_;
    const auto& sample_callback = state->sample_callback;
    const auto period = config.screenshot_period;
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
            if (sample_callback) {
                sample_callback(std::nullopt);
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
                if (sample_callback) {
                    sample_callback(std::nullopt);
                }
                next_sample = std::chrono::steady_clock::now() + kServiceRetryPeriod;
                continue;
            }
            const int32_t crop_size = (crop.right - crop.left) * (crop.bottom - crop.top);
            const bool weighted = config.screenshot_weighted && config.weights.size() == crop_size;
            LOG(INFO) << "FusionLight CWB geometry=" << width << 'x' << height << " crop=["
                      << crop.left << ',' << crop.top << ',' << crop.right << ',' << crop.bottom
                      << "] weighted=" << weighted;
            if (config.screenshot_weighted && !weighted) {
                LOG(WARNING) << "CWB crop size changed from the profile; using unweighted "
                                "sampling";
            }
        }

        uint64_t request;
        {
            std::lock_guard lock(state->mutex);
            request = ++state->active_request;
        }
        const auto frame_start = GetBootTime();
        const auto callback = ndk::SharedRefBase::make<DisplayCallback>(state, request);
        const auto request_status = service->setCWBOutputBuffer(
                callback, kPrimaryDisplay, crop, true, ::android::dupToAidl(buffer->handle));
        if (!request_status.isOk()) {
            LOG(WARNING) << "IDisplayConfig::setCWBOutputBuffer failed: "
                         << request_status.getDescription();
            service.reset();
            buffer.clear();
            if (sample_callback) {
                sample_callback(std::nullopt);
            }
            next_sample = std::chrono::steady_clock::now() + kServiceRetryPeriod;
            continue;
        }

        int32_t error;
        std::chrono::nanoseconds frame_end;
        bool completed;
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
                frame_end = state->completed_time;
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
            sample = ReadSample(buffer, crop, config, frame_start, frame_end);
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
