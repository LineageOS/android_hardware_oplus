/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "FusionLightProcessor.h"

#include <android-base/logging.h>
#include <android-base/properties.h>

namespace android {
namespace hardware {
namespace sensors {
namespace V2_1 {
namespace subhal {
namespace implementation {
namespace fusionlight {

using namespace std::chrono_literals;

namespace {

constexpr size_t kMaxScreenSamples = 5;
constexpr int64_t kSampleGracePeriods = 2;
constexpr int64_t kCaptureStallPeriods = 8;

struct PendingEvent {
    Event event;
    Channels raw_channels = Channels::Zero();
    int32_t brightness = 0;
};

int FindRange(const std::vector<ValueRange>& ranges, double value) {
    for (int level = 0; level < ranges.size(); ++level) {
        if (value >= ranges[level].min && value < ranges[level].max) {
            return level;
        }
    }
    // Some profile tables use adjacent inclusive integer ranges (for example,
    // 1..419 followed by 420..728). Recover their upper endpoint only when no
    // half-open range matched.
    for (int level = 0; level < ranges.size(); ++level) {
        if (value >= ranges[level].min && value <= ranges[level].max) {
            return level;
        }
    }
    return -1;
}

int FindIrBand(const std::vector<ValueRange>& thresholds, double ratio) {
    for (int level = 0; level < thresholds.size(); ++level) {
        if (ratio >= thresholds[level].min && ratio <= thresholds[level].max) {
            return level;
        }
    }
    return thresholds.empty() ? -1 : 0;
}

int FindChannelPolicy(const FusionConfig& config, const Channels& channels) {
    if (!config.channel_count_policy_supported) {
        return -1;
    }
    for (int level = 0; level < config.channel_thresholds.size(); ++level) {
        bool matches = true;
        for (int channel = 0; channel < kChannelCount; ++channel) {
            const int max = config.channel_thresholds[level][channel];
            matches &= max == -1 || channels[channel] < max;
        }
        if (matches) {
            return level;
        }
    }
    return -1;
}

int FindCZeroLevel(const FusionConfig& config, double clear) {
    int level = 0;
    for (int candidate = 0; candidate < config.c_zero_thresholds.size(); ++candidate) {
        if (clear >= config.c_zero_thresholds[candidate]) {
            level = candidate;
        }
    }
    return level;
}

double IrRatio(const Channels& channels) {
    if (channels[3] <= 0.0) {
        return 0.0;
    }
    return std::max((channels[0] + channels[1] + channels[2] - channels[3]) / channels[3] * 0.5,
                    0.0);
}

std::optional<double> CalculateLux(const FusionConfig& config, const Channels& channels,
                                   int32_t brightness, bool screen_off) {
    const int band = FindIrBand(config.ir_thresholds, IrRatio(channels));
    if (band < 0 || band >= kIrBandCount) {
        return std::nullopt;
    }

    const std::vector<Channels>* table;
    int level;
    if (screen_off) {
        if (!config.screen_off_lux_supported) {
            return std::nullopt;
        }
        table = &config.screen_off_coefficients[band];
        level = FindCZeroLevel(config, channels[3]);
    } else {
        level = FindChannelPolicy(config, channels);
        if (level >= 0) {
            table = &config.channel_count_coefficients[band];
        } else {
            table = &config.lux_coefficients[band];
            level = FindRange(config.ir_brightness, brightness);
        }
    }

    if (level < 0 || level >= table->size()) {
        return std::nullopt;
    }
    const double lux = channels.dot((*table)[level]);
    return std::isfinite(lux) ? std::optional(std::max(lux, 0.0)) : std::nullopt;
}

LeakageModel LeakageTerms(double linearity, double r, double g, double b) {
    const double l2 = linearity * linearity;
    const double r2 = r * r;
    const double g2 = g * g;
    const double b2 = b * b;
    LeakageModel terms;
    terms << 1.0, linearity, r, g, b, l2, linearity * r, linearity * g, linearity * b, r2, r * g,
            r * b, g2, g * b, b2, l2 * linearity, l2 * r, l2 * g, l2 * b, linearity * r2,
            linearity * r * g, linearity * r * b, linearity * g2, linearity * g * b, linearity * b2,
            r2 * r, r2 * g, r2 * b, r * g2, r * g * b, r * b2, g2 * g, g2 * b, g * b2, b2 * b;
    return terms;
}

RatioModel RatioTerms(double r, double g, double b) {
    const double r2 = r * r;
    const double g2 = g * g;
    const double b2 = b * b;
    RatioModel terms;
    terms << 1.0, r, g, b, r2, r * g, r * b, g2, g * b, b2, r2 * r, r2 * g, r2 * b, r * g2,
            r * g * b, r * b2, g2 * g, g2 * b, g * b2, b2 * b;
    return terms;
}

std::optional<double> CalculateCorrectedLux(const FusionConfig& config,
                                            const std::vector<Channels>& calibration,
                                            const PendingEvent& pending_event,
                                            const CwbSample& sample) {
    const Channels& raw_channels = pending_event.raw_channels;
    const int32_t brightness = pending_event.brightness;
    const int level = FindRange(config.linearity_brightness, brightness);
    if (level < 0 || level >= calibration.size()) {
        LOG(WARNING) << "FusionLight calibration invalid level ts_ns="
                     << pending_event.event.timestamp << " brightness=" << brightness
                     << " level=" << level;
        return std::nullopt;
    }

    const auto ratio_basis = RatioTerms(sample.r, sample.g, sample.b);
    Channels compensated;
    for (int channel = 0; channel < kChannelCount; ++channel) {
        const auto& polynomial = config.linearity[level][channel];
        const double x = brightness;
        const double linearity =
                ((polynomial[0] * x + polynomial[1]) * x + polynomial[2]) * x + polynomial[3];
        const auto leakage_basis = LeakageTerms(linearity, sample.r, sample.g, sample.b);
        const double parameter = config.leakage[level][channel].dot(leakage_basis);
        const double ratio = config.leakage_ratio[level][channel].dot(ratio_basis);
        const double leakage = std::max(parameter * ratio * calibration[level][channel], 0.0);
        compensated[channel] = std::max(raw_channels[channel] - leakage, 0.0);
    }
    return CalculateLux(config, compensated, brightness, false);
}

std::optional<CwbSample> SelectScreenSample(std::chrono::nanoseconds event_time,
                                            const std::deque<CwbSample>& samples) {
    for (auto sample = samples.rbegin(); sample != samples.rend(); ++sample) {
        if (sample->frame_end <= event_time) {
            return *sample;
        }
    }
    return std::nullopt;
}

Event MakeFusionLightEvent(const Event& event, int32_t fusion_light_handle, float lux) {
    Event result = event;
    result.sensorHandle = fusion_light_handle;
    result.sensorType = SensorType::LIGHT;
    result.u.scalar = lux;
    return result;
}

}  // namespace

struct FusionLightProcessor::SharedState {
    explicit SharedState(EmitCallback callback) : emit(std::move(callback)) {}

    bool startDispatchLocked() {
        if (dispatching || delivery.empty()) {
            return false;
        }
        dispatching = true;
        return true;
    }

    void enqueueDeliveryLocked(Event event) { delivery.push_back(std::move(event)); }

    void finishWaitingLocked() {
        if (!waiting_for_sample.has_value()) {
            return;
        }
        if (cwb_failed) {
            enqueueDeliveryLocked(std::move(waiting_for_sample->event));
        }
        waiting_for_sample.reset();
    }

    void checkCwbStallLocked(std::chrono::nanoseconds event_time) {
        const auto last_capture =
                screen_samples.empty() ? minimum_frame_start : screen_samples.back().frame_end;
        if (!cwb_failed && last_capture > 0ns && event_time > last_capture &&
            event_time - last_capture > config.cwb.screenshot_period * kCaptureStallPeriods) {
            LOG(WARNING) << "FusionLight CWB stalled since_ns=" << last_capture.count()
                         << " event_ns=" << event_time.count();
            cwb_failed = true;
            finishWaitingLocked();
        }
    }

    void calibrateWithSampleLocked(PendingEvent& pending_event, const CwbSample& sample) {
        const auto lux = CalculateCorrectedLux(config, calibration, pending_event, sample);
        if (lux.has_value()) {
            pending_event.event.u.scalar = *lux;
        } else {
            LOG(WARNING) << "FusionLight RAW ts_ns=" << pending_event.event.timestamp
                         << " brightness=" << pending_event.brightness
                         << " reason=invalid_calibration lux=" << pending_event.event.u.scalar;
        }
    }

    bool updateCalibrationLocked(PendingEvent& pending_event) {
        const auto event_time = std::chrono::nanoseconds(pending_event.event.timestamp);
        const auto sample = SelectScreenSample(event_time, screen_samples);
        if (!sample.has_value()) {
            return false;
        }
        const auto age = event_time - sample->frame_end;
        if (cwb_failed && age > config.cwb.screenshot_period * kSampleGracePeriods) {
            return false;
        }
        calibrateWithSampleLocked(pending_event, *sample);
        return true;
    }

    void finishWaitingWithSampleLocked() {
        if (!waiting_for_sample.has_value() || screen_samples.empty()) {
            return;
        }
        if (updateCalibrationLocked(*waiting_for_sample)) {
            enqueueDeliveryLocked(std::move(waiting_for_sample->event));
            waiting_for_sample.reset();
            return;
        }
        const auto& sample = screen_samples.back();
        const auto event_time = std::chrono::nanoseconds(waiting_for_sample->event.timestamp);
        // A pending event may use a later capture only within two sample periods.
        if (sample.frame_end >= event_time &&
            sample.frame_end - event_time <= config.cwb.screenshot_period * kSampleGracePeriods) {
            calibrateWithSampleLocked(*waiting_for_sample, sample);
            enqueueDeliveryLocked(std::move(waiting_for_sample->event));
            waiting_for_sample.reset();
            return;
        }
        finishWaitingLocked();
    }

    void dispatch() {
        for (;;) {
            Event event;
            EmitCallback callback;
            {
                std::lock_guard lock(mutex);
                if (delivery.empty()) {
                    dispatching = false;
                    return;
                }
                event = std::move(delivery.front());
                delivery.pop_front();
                callback = emit;
            }
            if (callback) {
                callback(std::move(event));
            }
        }
    }

    std::mutex mutex;
    EmitCallback emit;
    FusionConfig config;
    std::vector<Channels> calibration;
    std::optional<PendingEvent> waiting_for_sample;
    std::deque<Event> delivery;
    std::deque<CwbSample> screen_samples;
    std::chrono::nanoseconds minimum_frame_start{0};
    int32_t brightness = -1;
    bool active = false;
    bool available = false;
    bool cwb_failed = false;
    bool dispatching = false;
};

bool FusionLightProcessor::initialize(const std::string& sensor_name) {
    std::lock_guard lifecycle_lock(lifecycle_mutex_);
    profile_sensor_name_ = sensor_name;
    return initializeLocked();
}

bool FusionLightProcessor::initializeLocked() {
    if (state_->available) {
        return true;
    }

    if (!android::base::GetBoolProperty("ro.vendor.oplus.sensor.high_pwm_rgb", false)) {
        LOG(INFO) << "High-PWM RGB property is disabled; FusionLight will pass raw lux";
        return false;
    }

    auto config = profile_sensor_name_.empty() ? std::optional<FusionConfig>{}
                                               : LoadConfig(profile_sensor_name_);
    if (!config.has_value() || !config->cwb_supported || !config->fusion_rgb_supported ||
        !config->screenshot_v2_1_supported) {
        LOG(WARNING) << "FusionLight V2.1 is disabled or unavailable; passing raw lux";
        return false;
    }

    std::vector<Channels> calibration = LoadCalibration(*config);
    sampler_.setConfig(config->cwb);
    {
        std::lock_guard lock(state_->mutex);
        state_->config = std::move(*config);
        state_->calibration = std::move(calibration);
        state_->available = true;
    }
    return true;
}

void FusionLightProcessor::setEnabled(bool enabled) {
    std::lock_guard lifecycle_lock(lifecycle_mutex_);
    if (enabled) {
        initializeLocked();
        {
            std::lock_guard lock(state_->mutex);
            state_->active = true;
            state_->screen_samples.clear();
            state_->waiting_for_sample.reset();
            state_->delivery.clear();
            state_->minimum_frame_start = 0ns;
            state_->brightness = -1;
            state_->cwb_failed = false;
        }
        return;
    }

    {
        std::lock_guard lock(state_->mutex);
        state_->active = false;
    }
    sampler_.stop();
    {
        std::lock_guard lock(state_->mutex);
        state_->screen_samples.clear();
        state_->waiting_for_sample.reset();
        state_->delivery.clear();
        state_->minimum_frame_start = 0ns;
        state_->brightness = -1;
        state_->cwb_failed = false;
    }
}

void FusionLightProcessor::enqueueReady(Event event) {
    bool should_dispatch = false;
    {
        std::lock_guard lock(state_->mutex);
        if (!state_->active) {
            return;
        }
        state_->finishWaitingLocked();
        state_->enqueueDeliveryLocked(std::move(event));
        should_dispatch = state_->startDispatchLocked();
    }
    if (should_dispatch) {
        state_->dispatch();
    }
}

void FusionLightProcessor::processScreenOff(Event event, const Channels& raw_channels,
                                            int32_t brightness) {
    bool should_dispatch = false;
    {
        std::lock_guard lifecycle_lock(lifecycle_mutex_);
        sampler_.stop();
        {
            std::lock_guard lock(state_->mutex);
            if (!state_->active) {
                return;
            }
            const auto lux = CalculateLux(state_->config, raw_channels, brightness, true);
            if (lux.has_value()) {
                event.u.scalar = *lux;
            }
            state_->waiting_for_sample.reset();
            state_->enqueueDeliveryLocked(std::move(event));
            should_dispatch = state_->startDispatchLocked();
            state_->screen_samples.clear();
            state_->minimum_frame_start = 0ns;
            state_->brightness = brightness;
            state_->cwb_failed = false;
        }
    }
    if (should_dispatch) {
        state_->dispatch();
    }
}

void FusionLightProcessor::process(const Event& event, int32_t fusion_light_handle) {
    const auto event_time = std::chrono::nanoseconds(event.timestamp);
    Event fusion_light_event = MakeFusionLightEvent(event, fusion_light_handle, event.u.data[0]);

    Channels raw_channels;
    for (int channel = 0; channel < kChannelCount; ++channel) {
        raw_channels[channel] = event.u.data[channel + 4];
        if (!std::isfinite(raw_channels[channel])) {
            enqueueReady(std::move(fusion_light_event));
            return;
        }
    }
    const int32_t brightness = event.u.data[3];

    bool available;
    {
        std::lock_guard lock(state_->mutex);
        if (!state_->active) {
            return;
        }
        available = state_->available;
    }
    if (!available) {
        enqueueReady(std::move(fusion_light_event));
        return;
    }
    if (brightness <= 0) {
        processScreenOff(std::move(fusion_light_event), raw_channels, brightness);
        return;
    }

    bool brightness_changed;
    bool start_sampler;
    {
        std::lock_guard lifecycle_lock(lifecycle_mutex_);
        {
            std::lock_guard lock(state_->mutex);
            if (!state_->active) {
                return;
            }
            brightness_changed = brightness != state_->brightness;
            start_sampler = state_->brightness <= 0;
            if (start_sampler) {
                state_->minimum_frame_start = event_time;
                state_->cwb_failed = false;
            }
            state_->checkCwbStallLocked(event_time);
            if (brightness_changed) {
                state_->finishWaitingLocked();
                state_->brightness = brightness;
            }
        }
        if (start_sampler) {
            sampler_.start();
        }
        if (brightness_changed) {
            sampler_.requestSample();
        }
    }

    bool should_dispatch = false;
    {
        std::lock_guard lock(state_->mutex);
        if (!state_->active) {
            return;
        }
        state_->finishWaitingLocked();
        PendingEvent pending_event{std::move(fusion_light_event), raw_channels, brightness};
        if (state_->updateCalibrationLocked(pending_event)) {
            state_->enqueueDeliveryLocked(std::move(pending_event.event));
        } else if (state_->cwb_failed) {
            state_->enqueueDeliveryLocked(std::move(pending_event.event));
        } else {
            state_->waiting_for_sample = std::move(pending_event);
        }
        should_dispatch = state_->startDispatchLocked();
    }
    if (should_dispatch) {
        state_->dispatch();
    }
}

void FusionLightProcessor::completeFlush(Event event) {
    bool should_dispatch = false;
    {
        std::lock_guard lock(state_->mutex);
        state_->finishWaitingLocked();
        state_->enqueueDeliveryLocked(std::move(event));
        should_dispatch = state_->startDispatchLocked();
    }
    if (should_dispatch) {
        state_->dispatch();
    }
}

FusionLightProcessor::FusionLightProcessor(EmitCallback emit_callback)
    : state_(std::make_shared<SharedState>(std::move(emit_callback))),
      sampler_([state = std::weak_ptr<SharedState>(state_)](std::optional<CwbSample> sample) {
          const auto shared_state = state.lock();
          if (shared_state == nullptr) {
              return;
          }

          bool should_dispatch = false;
          {
              std::lock_guard lock(shared_state->mutex);
              if (!shared_state->active) {
                  return;
              }
              if (!sample.has_value() || sample->frame_start <= 0ns ||
                  sample->frame_end <= sample->frame_start) {
                  shared_state->cwb_failed = true;
                  shared_state->finishWaitingLocked();
              } else if (sample->frame_start >= shared_state->minimum_frame_start) {
                  shared_state->cwb_failed = false;
                  shared_state->screen_samples.push_back(*sample);
                  while (shared_state->screen_samples.size() > kMaxScreenSamples) {
                      shared_state->screen_samples.pop_front();
                  }
                  shared_state->finishWaitingWithSampleLocked();
              }
              should_dispatch = shared_state->startDispatchLocked();
          }
          if (should_dispatch) {
              shared_state->dispatch();
          }
      }) {}

FusionLightProcessor::~FusionLightProcessor() {
    setEnabled(false);
    std::lock_guard lock(state_->mutex);
    state_->emit = nullptr;
}

}  // namespace fusionlight
}  // namespace implementation
}  // namespace subhal
}  // namespace V2_1
}  // namespace sensors
}  // namespace hardware
}  // namespace android
