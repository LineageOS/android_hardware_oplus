/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android/binder_auto_utils.h>
#include <android/binder_manager.h>
#include <Eigen/Core>

namespace android {
namespace hardware {
namespace sensors {
namespace V2_1 {
namespace subhal {
namespace implementation {
namespace fusionlight {

constexpr int kChannelCount = 4;
constexpr int kIrBandCount = 3;
constexpr int kLeakageTermCount = 35;
constexpr int kRatioTermCount = 20;

using Channels = Eigen::Matrix<double, kChannelCount, 1>;
using ChannelLimits = Eigen::Matrix<int, kChannelCount, 1>;
using Polynomial = Eigen::Vector4d;
using LeakageModel = Eigen::Matrix<double, kLeakageTermCount, 1>;
using RatioModel = Eigen::Matrix<double, kRatioTermCount, 1>;

struct ValueRange {
    double min = 0.0;
    double max = 0.0;
};

struct ChannelThreshold {
    ChannelLimits max = ChannelLimits::Zero();
};

struct FusionConfig {
    bool fusion_rgb_supported = false;
    bool cwb_supported = false;
    bool cwb_screenshot_weighted = false;
    bool screenshot_v2_1_supported = false;
    bool screen_off_lux_supported = false;
    bool channel_count_policy_supported = false;

    int32_t crop_left = 0;
    int32_t crop_top = 0;
    int32_t crop_right = 0;
    int32_t crop_bottom = 0;
    int32_t reference_width = 0;
    int32_t reference_height = 0;
    int32_t sample_period_for_60_ns = 0;
    int32_t cwb_screenshot_cal_delay_ns = 0;
    int32_t cwb_screenshot_cal_delay_90_ns = 0;
    int32_t cwb_screenshot_cal_delay_60_ns = 0;
    int32_t cwb_screenshot_period_millis = 0;
    double cwb_screenshot_match_ratio_threshold = 0.0;
    std::vector<int32_t> cwb_weights;

    std::vector<ValueRange> linearity_brightness;
    std::vector<ValueRange> ir_brightness;
    std::vector<ValueRange> ir_thresholds;
    std::vector<std::array<Polynomial, kChannelCount>> linearity;
    std::vector<std::array<LeakageModel, kChannelCount>> leakage;
    std::vector<std::array<RatioModel, kChannelCount>> leakage_ratio;
    std::vector<Channels> leakage_golden;

    std::array<std::vector<Channels>, kIrBandCount> lux_coefficients;
    std::array<std::vector<Channels>, kIrBandCount> channel_count_coefficients;
    std::array<std::vector<Channels>, kIrBandCount> screen_off_coefficients;
    std::vector<ChannelThreshold> channel_thresholds;
    std::vector<double> c_zero_thresholds;
};

std::optional<FusionConfig> LoadConfig(const std::string& sensor_name);
std::vector<Channels> LoadCalibration(const FusionConfig& config);

template <typename Interface>
std::shared_ptr<Interface> GetService() {
    const std::string instance = std::string(Interface::descriptor) + "/default";
    ndk::SpAIBinder binder(AServiceManager_checkService(instance.c_str()));
    return binder.get() == nullptr ? nullptr : Interface::fromBinder(binder);
}

}  // namespace fusionlight
}  // namespace implementation
}  // namespace subhal
}  // namespace V2_1
}  // namespace sensors
}  // namespace hardware
}  // namespace android
