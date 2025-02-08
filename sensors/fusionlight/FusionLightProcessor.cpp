/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "FusionLightProcessor.h"

#include <aidl/vendor/lineage/oplus_fusionlight/BnScreenSampleCallback.h>
#include <android-base/logging.h>
#include <android-base/properties.h>

namespace android {
namespace hardware {
namespace sensors {
namespace V2_1 {
namespace subhal {
namespace implementation {
namespace fusionlight {
namespace {

using namespace std::chrono_literals;

using aidl::vendor::lineage::oplus_fusionlight::BnScreenSampleCallback;
using aidl::vendor::lineage::oplus_fusionlight::IScreenSampler;
using aidl::vendor::lineage::oplus_fusionlight::SamplingConfig;
using aidl::vendor::lineage::oplus_fusionlight::ScreenSample;

constexpr auto kServiceRetryPeriod = 2s;
constexpr size_t kMaxScreenSamples = 5;
constexpr int64_t kMaxScreenSampleAgePeriods = 3;
constexpr auto k90HzPeriodLowerBound = 10ms;
constexpr auto k90HzPeriodUpperBound = 12ms;
constexpr auto k60HzPeriodLowerBound = 15ms;
constexpr auto k60HzPeriodUpperBound = 17ms;
constexpr auto kFallback60HzSamplePeriod = 8333300ns;

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
            const int max = config.channel_thresholds[level].max[channel];
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
            r * b, g2, g * b, b2, std::sqrt(std::max(linearity, 0.0)), l2 * r, l2 * g, l2 * b,
            linearity * r2, linearity * r * g, linearity * r * b, linearity * g2, linearity * g * b,
            linearity * b2, r2 * r, r2 * g, r2 * b, r * g2, r * g * b, r * b2, g2 * g, g2 * b,
            g * b2, b2 * b;
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
                                            const Channels& raw_channels, int32_t brightness,
                                            const ScreenSample& sample) {
    const int level = FindRange(config.linearity_brightness, brightness);
    if (level < 0 || level >= calibration.size()) {
        return std::nullopt;
    }

    const auto ratio_basis = RatioTerms(sample.r, sample.g, sample.b);
    Channels compensated = Channels::Zero();
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

bool IsPeriodInRange(std::chrono::nanoseconds period, std::chrono::nanoseconds lower_bound,
                     std::chrono::nanoseconds upper_bound) {
    return period > lower_bound && period < upper_bound;
}

int64_t GetSensorPeriodNs(const Event& event) {
    const std::chrono::duration<double, std::milli> period(event.u.data[1]);
    if (!std::isfinite(period.count()) || period <= 0ms) {
        return 0;
    }
    return std::chrono::duration_cast<std::chrono::nanoseconds>(period).count();
}

double GetMatchRatio(const FusionConfig& config, const Event& event, const ScreenSample& sample) {
    const int64_t sensor_period_ns = GetSensorPeriodNs(event);
    if (sensor_period_ns <= 0 || sample.frameEndNs <= sample.frameStartNs) {
        return 0.0;
    }

    int64_t matching_period_ns = sensor_period_ns;
    int32_t calibration_delay_ns = config.sf_screenshot_cal_delay_ns;
    if (IsPeriodInRange(std::chrono::nanoseconds(sensor_period_ns), k90HzPeriodLowerBound,
                        k90HzPeriodUpperBound)) {
        calibration_delay_ns = config.sf_screenshot_cal_delay_90_ns;
    } else if (IsPeriodInRange(std::chrono::nanoseconds(sensor_period_ns), k60HzPeriodLowerBound,
                               k60HzPeriodUpperBound)) {
        const int64_t frame_period_ns = sample.frameEndNs - sample.frameStartNs;
        if (config.sf_screenshot_cal_delay_60_ns != -1 &&
            IsPeriodInRange(std::chrono::nanoseconds(frame_period_ns), k60HzPeriodLowerBound,
                            k60HzPeriodUpperBound)) {
            calibration_delay_ns = config.sf_screenshot_cal_delay_60_ns;
            matching_period_ns = config.sample_period_for_60_ns;
        } else {
            matching_period_ns = kFallback60HzSamplePeriod.count();
        }
    }

    const int64_t sensor_start_ns = event.timestamp - sensor_period_ns;
    const int64_t sensor_end_ns = sensor_start_ns + matching_period_ns;
    const int64_t frame_start_ns = sample.frameStartNs + calibration_delay_ns;
    const int64_t frame_end_ns = sample.frameEndNs + calibration_delay_ns;
    const int64_t overlap_ns =
            std::min(sensor_end_ns, frame_end_ns) - std::max(sensor_start_ns, frame_start_ns);
    return overlap_ns > 0 ? static_cast<double>(overlap_ns) / matching_period_ns : 0.0;
}

std::optional<ScreenSample> SelectScreenSample(const FusionConfig& config, const Event& event,
                                               const std::deque<ScreenSample>& samples) {
    double best_ratio = config.sf_screenshot_match_ratio_threshold;
    std::optional<ScreenSample> best_sample;
    for (const auto& sample : samples) {
        const double ratio = GetMatchRatio(config, event, sample);
        if (ratio >= best_ratio) {
            best_ratio = ratio;
            best_sample = sample;
        }
    }
    if (best_sample.has_value() || samples.empty()) {
        return best_sample;
    }

    const auto max_age = std::chrono::milliseconds(
            static_cast<int64_t>(config.sf_screenshot_period_millis) * kMaxScreenSampleAgePeriods);
    const auto& newest_sample = samples.back();
    if (event.timestamp > newest_sample.frameEndNs &&
        std::chrono::nanoseconds(event.timestamp - newest_sample.frameEndNs) > max_age) {
        return std::nullopt;
    }
    if (newest_sample.frameStartNs > event.timestamp &&
        std::chrono::nanoseconds(newest_sample.frameStartNs - event.timestamp) > max_age) {
        return std::nullopt;
    }
    // ALS events and periodic screenshots are independently scheduled, so an
    // exact overlap is not guaranteed. The newest frame is still the closest
    // available representation of the current display contents.
    return newest_sample;
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

    void finishWaitingRawLocked() {
        if (waiting_for_sample.has_value()) {
            enqueueDeliveryLocked(std::move(waiting_for_sample->event));
            waiting_for_sample.reset();
        }
    }

    bool updateCalibrationLocked(PendingEvent& pending_event) {
        const auto sample = SelectScreenSample(config, pending_event.event, screen_samples);
        if (!sample.has_value()) {
            return false;
        }
        const auto lux = CalculateCorrectedLux(config, calibration, pending_event.raw_channels,
                                               pending_event.brightness, *sample);
        if (lux.has_value()) {
            pending_event.event.u.scalar = static_cast<float>(*lux);
        }
        return true;
    }

    bool finishWaitingWithSampleLocked() {
        if (!waiting_for_sample.has_value() || !updateCalibrationLocked(*waiting_for_sample)) {
            return false;
        }
        enqueueDeliveryLocked(std::move(waiting_for_sample->event));
        waiting_for_sample.reset();
        return true;
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
    std::deque<ScreenSample> screen_samples;
    int64_t session_id = -1;
    bool active = false;
    bool available = false;
    bool dispatching = false;
};

class FusionLightProcessor::SampleCallback final : public BnScreenSampleCallback {
  public:
    explicit SampleCallback(std::weak_ptr<SharedState> state) : state_(std::move(state)) {}

    ndk::ScopedAStatus onSample(const ScreenSample& sample) override {
        const auto state = state_.lock();
        if (state == nullptr) {
            return ndk::ScopedAStatus::ok();
        }

        bool should_dispatch = false;
        {
            std::lock_guard lock(state->mutex);
            if (!state->active || sample.sessionId != state->session_id ||
                sample.frameEndNs <= sample.frameStartNs) {
                return ndk::ScopedAStatus::ok();
            }
            state->screen_samples.push_back(sample);
            while (state->screen_samples.size() > kMaxScreenSamples) {
                state->screen_samples.pop_front();
            }
            state->finishWaitingWithSampleLocked();
            should_dispatch = state->startDispatchLocked();
        }
        if (should_dispatch) {
            state->dispatch();
        }
        return ndk::ScopedAStatus::ok();
    }

  private:
    std::weak_ptr<SharedState> state_;
};

bool FusionLightProcessor::initialize(const std::string& sensor_name) {
    std::lock_guard lifecycle_lock(lifecycle_mutex_);
    profile_sensor_name_ = sensor_name;
    return initializeLocked();
}

bool FusionLightProcessor::initializeLocked() {
    {
        std::lock_guard lock(state_->mutex);
        if (state_->available) {
            return true;
        }
    }

    if (!android::base::GetBoolProperty("ro.vendor.oplus.sensor.high_pwm_rgb", false)) {
        LOG(INFO) << "High-PWM RGB property is disabled; FusionLight will pass raw lux";
        return false;
    }

    auto config = profile_sensor_name_.empty() ? std::optional<FusionConfig>{}
                                               : LoadConfig(profile_sensor_name_);
    if (!config.has_value() || !config->fusion_rgb_supported ||
        !config->screenshot_v2_1_supported) {
        LOG(WARNING) << "FusionLight V2.1 is disabled or unavailable; passing raw lux";
        return false;
    }

    std::vector<Channels> calibration = LoadCalibration(*config);
    {
        std::lock_guard lock(state_->mutex);
        state_->config = std::move(*config);
        state_->calibration = std::move(calibration);
        state_->available = true;
    }
    return true;
}

void FusionLightProcessor::setEnabled(bool enabled) {
    if (enabled) {
        {
            std::lock_guard lifecycle_lock(lifecycle_mutex_);
            initializeLocked();
        }
        {
            std::lock_guard lock(state_->mutex);
            state_->active = true;
            state_->screen_samples.clear();
            state_->waiting_for_sample.reset();
            state_->delivery.clear();
        }
        ensureSession();
        return;
    }

    std::shared_ptr<IScreenSampler> service;
    int64_t session_id = -1;
    {
        std::lock_guard lifecycle_lock(lifecycle_mutex_);
        service = std::move(sampler_);
        callback_.reset();
        session_id = session_;
        session_ = -1;
        std::lock_guard lock(state_->mutex);
        state_->active = false;
        state_->session_id = -1;
        state_->screen_samples.clear();
        state_->waiting_for_sample.reset();
        state_->delivery.clear();
    }
    if (service != nullptr && session_id > 0) {
        service->stopSampling(session_id);
    }
}

bool FusionLightProcessor::ensureSession() {
    std::lock_guard lifecycle_lock(lifecycle_mutex_);
    if (session_ > 0) {
        if (sampler_ != nullptr) {
            const ndk::SpAIBinder binder = sampler_->asBinder();
            if (binder.get() != nullptr && AIBinder_isAlive(binder.get())) {
                return true;
            }
        }
        sampler_.reset();
        callback_.reset();
        session_ = -1;
        std::lock_guard lock(state_->mutex);
        state_->session_id = -1;
        state_->screen_samples.clear();
    }

    {
        std::lock_guard lock(state_->mutex);
        if (!state_->active || !state_->available) {
            return false;
        }
    }

    const auto now = std::chrono::steady_clock::now();
    if (now < next_service_attempt_) {
        return false;
    }
    next_service_attempt_ = now + kServiceRetryPeriod;

    sampler_ = GetService<IScreenSampler>();
    if (sampler_ == nullptr) {
        return false;
    }

    SamplingConfig sampling_config;
    {
        std::lock_guard lock(state_->mutex);
        sampling_config.left = state_->config.crop_left;
        sampling_config.top = state_->config.crop_top;
        sampling_config.right = state_->config.crop_right;
        sampling_config.bottom = state_->config.crop_bottom;
        sampling_config.referenceWidth = state_->config.reference_width;
        sampling_config.referenceHeight = state_->config.reference_height;
        sampling_config.samplingPeriodMillis = state_->config.sf_screenshot_period_millis;
    }

    callback_ = ndk::SharedRefBase::make<SampleCallback>(state_);
    int64_t new_session = -1;
    const auto status = sampler_->startSampling(sampling_config, callback_, &new_session);
    if (!status.isOk() || new_session <= 0) {
        LOG(WARNING) << "Unable to start FusionLight screenshot sampling";
        sampler_.reset();
        callback_.reset();
        return false;
    }

    session_ = new_session;
    {
        std::lock_guard lock(state_->mutex);
        state_->session_id = session_;
        state_->screen_samples.clear();
    }
    return true;
}

void FusionLightProcessor::enqueueReady(Event event) {
    bool should_dispatch = false;
    {
        std::lock_guard lock(state_->mutex);
        if (!state_->active) {
            return;
        }
        state_->finishWaitingRawLocked();
        state_->enqueueDeliveryLocked(std::move(event));
        should_dispatch = state_->startDispatchLocked();
    }
    if (should_dispatch) {
        state_->dispatch();
    }
}

void FusionLightProcessor::processScreenOff(Event event, const Channels& raw_channels,
                                            int32_t brightness) {
    std::shared_ptr<IScreenSampler> service;
    int64_t session_id = -1;
    bool should_dispatch = false;
    {
        std::lock_guard lifecycle_lock(lifecycle_mutex_);
        std::lock_guard lock(state_->mutex);
        if (!state_->active) {
            return;
        }
        if (const auto lux = CalculateLux(state_->config, raw_channels, brightness, true);
            lux.has_value()) {
            event.u.scalar = static_cast<float>(*lux);
        }
        state_->waiting_for_sample.reset();
        state_->enqueueDeliveryLocked(std::move(event));
        should_dispatch = state_->startDispatchLocked();
        service = std::move(sampler_);
        callback_.reset();
        session_id = session_;
        session_ = -1;
        state_->session_id = -1;
        state_->screen_samples.clear();
        next_service_attempt_ = {};
    }
    if (should_dispatch) {
        state_->dispatch();
    }
    if (service != nullptr && session_id > 0) {
        service->stopSampling(session_id);
    }
}

void FusionLightProcessor::process(const Event& event, int32_t fusion_light_handle) {
    const float raw_lux = event.u.data[0];
    Event fusion_light_event = MakeFusionLightEvent(event, fusion_light_handle, raw_lux);

    Channels raw_channels = Channels::Zero();
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

    const bool has_session = ensureSession();

    bool should_dispatch = false;
    {
        std::lock_guard lock(state_->mutex);
        if (!state_->active) {
            return;
        }
        if (has_session) {
            state_->finishWaitingWithSampleLocked();
            PendingEvent pending_event{std::move(fusion_light_event), raw_channels, brightness};
            if (state_->updateCalibrationLocked(pending_event)) {
                state_->enqueueDeliveryLocked(std::move(pending_event.event));
            } else {
                state_->finishWaitingRawLocked();
                state_->waiting_for_sample = std::move(pending_event);
            }
        } else {
            state_->finishWaitingRawLocked();
            state_->enqueueDeliveryLocked(std::move(fusion_light_event));
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
        state_->finishWaitingRawLocked();
        state_->enqueueDeliveryLocked(std::move(event));
        should_dispatch = state_->startDispatchLocked();
    }
    if (should_dispatch) {
        state_->dispatch();
    }
}

FusionLightProcessor::FusionLightProcessor(EmitCallback emit_callback)
    : state_(std::make_shared<SharedState>(std::move(emit_callback))) {}

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
