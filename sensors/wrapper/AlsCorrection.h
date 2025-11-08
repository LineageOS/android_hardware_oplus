/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lineage/oplus_als/BnAreaCapture.h>
#include <android/hardware/sensors/2.1/types.h>

#include <utils/Timers.h>

namespace android {
namespace hardware {
namespace sensors {
namespace V2_1 {
namespace subhal {
namespace implementation {
namespace wrapper {

struct LuxCoeff {
    float r, g, b, c;
};

struct LinearityParams {
    float p0, p1, p2, p3;
};

struct LinearityFunction {
    int function;
    LinearityParams channels[4];  // RGBC
};

struct BrightnessRange {
    int level;
    int bright_min, bright_max;
};

struct IRThreshold {
    int level;
    float ir_min, ir_max;
};

struct IRBrightness {
    int level;
    int bright_min, bright_max;
};

struct Golden {
    int channel;
    int r, g, b, w;
};

struct GrayScale {
    int channel;
    float r, g, b;
};

struct PureColorLeak {
    int color_id;
    std::string color_name;
    int grey_scale_delta_threshold;
    float leak_ratio_min, leak_ratio_max;
};

struct CCTSegment {
    int level;
    int lux_min, lux_max;
    float max_leak_ratio_threshold;
    float leak_ratio_threshold;
    // GREY, RED, GREEN, BLUE, YELLOW, PURPLE, CYAN, WHITE_BLACK, LOW_COLOR_SCALE:
    PureColorLeak pure_colors[9];
    int pure_color_count;
};

struct ScreenShotRect {
    int left_top_x, left_top_y;
    int right_bottom_x, right_bottom_y;
};

struct CommonConfig {
    ScreenShotRect screenshot_rect;
    int brightness_max;
    int normal_brightness_max;
    bool fusion_rgb_supported;
};

struct FusionLightConfig {
    CommonConfig common;

    // CCT leakage model
    CCTSegment cct_segments[5];
    int cct_segment_count;

    // IR configuration
    IRThreshold ir_thresholds[3];
    IRBrightness ir_brightness[3];

    // Lux coefficients for default mode
    LuxCoeff lux_coeff_lir[3];
    LuxCoeff lux_coeff_hir[3];
    LuxCoeff lux_coeff_super_hir[3];

    // Linearity correction
    int linearity_type;
    BrightnessRange linearity_ranges[9];
    LinearityFunction linearity[9];

    // Calibration values
    Golden golden[4];
    GrayScale grayscale[4];

    // Display mode variants (L_ and M_ prefixed)
    bool has_l_mode;
    bool has_m_mode;
    IRBrightness l_ir_brightness[3];
    IRBrightness m_ir_brightness[3];
    LuxCoeff l_lux_coeff_lir[3];
    LuxCoeff m_lux_coeff_lir[3];
};

using aidl::vendor::lineage::oplus_als::IAreaCapture;

class AlsCorrection {
  public:
    bool init();
    float process(const Event& event);

  private:
    bool loadFusionLightConfig();
    float applyLinearityCorrection(float raw_value, int brightness, int channel);
    int determineIRLevel(float brightness, float ir_ratio, const IRBrightness* ir_bright);

    FusionLightConfig conf_;
    std::shared_ptr<IAreaCapture> service_ = nullptr;

    nsecs_t last_update_ = 0, last_forced_update_;
    bool force_update_;
    float hyst_min_ = -1.f, hyst_max_ = -1.f;
    float last_corrected_value_;

    struct {
        float middle;
        float min, max;
    } hysteresis_ranges_[10] = {
            {0, 0, 4},
            {7, 1, 12},
            {15, 5, 30},
            {30, 10, 50},
            {360, 25, 700},
            {1200, 300, 1600},
            {2250, 1000, 2940},
            {4600, 2000, 5900},
            {10000, 4000, 80000},
            {HUGE_VALF, 8000, HUGE_VALF},
    };
};

}  // namespace wrapper
}  // namespace implementation
}  // namespace subhal
}  // namespace V2_1
}  // namespace sensors
}  // namespace hardware
}  // namespace android
