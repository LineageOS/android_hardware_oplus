/*
 * Copyright (C) 2021-2024 The LineageOS Project
 * Copyright (C) 2025 The YAAP Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "AlsCorrection.h"

#include <android-base/properties.h>
#include <android/binder_manager.h>
#include <cmath>
#include <fstream>
#include <log/log.h>
#include <utils/Timers.h>
#include <json/json.h>

using aidl::vendor::lineage::oplus_als::AreaRgbCaptureResult;
using aidl::vendor::lineage::oplus_als::IAreaCapture;
using android::base::GetBoolProperty;

#define BRIGHTNESS_DIR "/sys/class/backlight/panel0-backlight/"
#define FUSIONLIGHT_JSON "/odm/etc/fusionlight.json"

namespace android {
namespace hardware {
namespace sensors {
namespace V2_1 {
namespace implementation {

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
    //GREY, RED, GREEN, BLUE, YELLOW, PURPLE, CYAN, WHITE_BLACK, LOW_COLOR_SCALE:
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

static FusionLightConfig conf;
static std::shared_ptr<IAreaCapture> service;

static struct {
    nsecs_t last_update, last_forced_update;
    bool force_update;
    float hyst_min, hyst_max;
    float last_corrected_value;
} state = {
    .last_update = 0,
    .force_update = true,
    .hyst_min = -1.0, .hyst_max = -1.0,
};

static struct {
    float middle;
    float min, max;
} hysteresis_ranges[] = {
    { 0, 0, 4 },
    { 7, 1, 12 },
    { 15, 5, 30 },
    { 30, 10, 50 },
    { 360, 25, 700 },
    { 1200, 300, 1600 },
    { 2250, 1000, 2940 },
    { 4600, 2000, 5900 },
    { 10000, 4000, 80000 },
    { HUGE_VALF, 8000, HUGE_VALF },
};

template <typename T>
static T get(const std::string& path, const T& def) {
    std::ifstream file(path);
    T result;
    file >> result;
    return file.fail() ? def : result;
}

static float parseFloat(const Json::Value& val) {
    if (val.isString()) {
        return std::stof(val.asString());
    }
    return val.asFloat();
}

static int parseInt(const Json::Value& val) {
    if (val.isString()) {
        return std::stoi(val.asString());
    }
    return val.asInt();
}

static bool loadFusionLightConfig() {
    std::ifstream ifs(FUSIONLIGHT_JSON);
    if (!ifs.is_open()) {
        ALOGE("Failed to open %s", FUSIONLIGHT_JSON);
        return false;
    }

    Json::CharReaderBuilder rbuilder;
    rbuilder["collectComments"] = false;
    std::string errs;
    Json::Value doc;

    if (!Json::parseFromStream(rbuilder, ifs, &doc, &errs)) {
        ALOGE("JSON parse error: %s", errs.c_str());
        return false;
    }

    // Parse CommonConfig
    if (doc.isMember("CommonConfig")) {
        const Json::Value& cc = doc["CommonConfig"];
        if (cc.isMember("ScreenShotRect")) {
            const Json::Value& rect = cc["ScreenShotRect"];
            conf.common.screenshot_rect.left_top_x = rect["LeftTopX"].asInt();
            conf.common.screenshot_rect.left_top_y = rect["LeftTopY"].asInt();
            conf.common.screenshot_rect.right_bottom_x = rect["RightBottomX"].asInt();
            conf.common.screenshot_rect.right_bottom_y = rect["RightBottomY"].asInt();
        }
        conf.common.brightness_max = cc.get("BrightnessMax", 4095).asInt();
        conf.common.normal_brightness_max = cc.get("NormalModeBrightnessMax", 3332).asInt();
        conf.common.fusion_rgb_supported = cc.get("FusionRGBSupported", true).asBool();
    }

    // Parse CCT leakage
    conf.cct_segment_count = 0;
    if (doc.isMember("CCTSegmentRange") && doc.isMember("CCTSegmentPureColorParameter")) {
        const Json::Value& seg_range = doc["CCTSegmentRange"];
        const Json::Value& seg_pure = doc["CCTSegmentPureColorParameter"];

        for (Json::ArrayIndex seg_idx = 0; seg_idx < seg_range.size() && seg_idx < 5; seg_idx++) {
            const Json::Value& seg = seg_range[seg_idx];
            int level = seg["Level"].asInt();
            conf.cct_segments[level].level = level;
            conf.cct_segments[level].lux_min = seg["LuxMin"].asInt();
            conf.cct_segments[level].lux_max = seg["LuxMax"].asInt();
            conf.cct_segments[level].max_leak_ratio_threshold = parseFloat(seg["CCTMaxLeakRatioThreshold"]);
            conf.cct_segments[level].leak_ratio_threshold = parseFloat(seg["CCTLeakRatioThreshold"]);
            // Find matching pure color params by Level field (not array index!)
            conf.cct_segments[level].pure_color_count = 0;
            for (Json::ArrayIndex pure_idx = 0; pure_idx < seg_pure.size(); pure_idx++) {
                if (seg_pure[pure_idx]["Level"].asInt() == level &&
                    seg_pure[pure_idx].isMember("CCTPureColorParameter")) {
                        const Json::Value& colors = seg_pure[pure_idx]["CCTPureColorParameter"];
                        for (Json::ArrayIndex color_idx = 0; color_idx < colors.size() && color_idx < 9; color_idx++) {
                            const Json::Value& col = colors[color_idx];
                            auto& pc = conf.cct_segments[level].pure_colors[color_idx];
                            pc.color_id = col["PureColorId"].asInt();
                            pc.color_name = col["PureColorName"].asString();
                            pc.grey_scale_delta_threshold = col["GreyScaleDeltaThreshold"].asInt();
                            pc.leak_ratio_min = parseFloat(col["LeakRatioMin"]);
                            pc.leak_ratio_max = parseFloat(col["LeakRatioMax"]);
                            conf.cct_segments[level].pure_color_count++;
                        }
                    break;
                }
            }
            conf.cct_segment_count++;
        }
    }

    if (conf.cct_segment_count > 0) {
        ALOGI("Loaded %d CCT leak segments:", conf.cct_segment_count);
        for (int i = 0; i < conf.cct_segment_count; i++) {
            ALOGI("  Seg%d: lux %d-%d, leak thresh %.3f-%.3f, %d pure colors",
                  conf.cct_segments[i].level,
                  conf.cct_segments[i].lux_min, conf.cct_segments[i].lux_max,
                  conf.cct_segments[i].leak_ratio_threshold,
                  conf.cct_segments[i].max_leak_ratio_threshold,
                  conf.cct_segments[i].pure_color_count);
        }
    }

    // Parse IRThreshold
    if (doc.isMember("IRThreshold")) {
        const Json::Value& arr = doc["IRThreshold"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 3; i++) {
            const Json::Value& item = arr[i];
            conf.ir_thresholds[i].level = item["Level"].asInt();
            conf.ir_thresholds[i].ir_min = parseFloat(item["IR_Ratio_Min"]);
            conf.ir_thresholds[i].ir_max = parseFloat(item["IR_Ratio_Max"]);
        }
    }

    // Parse IRBrightness
    if (doc.isMember("IRBrightness")) {
        const Json::Value& arr = doc["IRBrightness"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 3; i++) {
            const Json::Value& item = arr[i];
            conf.ir_brightness[i].level = item["Level"].asInt();
            conf.ir_brightness[i].bright_min = item["BrightnessMin"].asInt();
            conf.ir_brightness[i].bright_max = item["BrightnessMax"].asInt();
        }
    }

    // Parse LuxCoeffLIR
    if (doc.isMember("LuxCoeffLIR")) {
        const Json::Value& arr = doc["LuxCoeffLIR"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 3; i++) {
            const Json::Value& item = arr[i];
            conf.lux_coeff_lir[i].r = parseFloat(item["ChannelR"]);
            conf.lux_coeff_lir[i].g = parseFloat(item["ChannelG"]);
            conf.lux_coeff_lir[i].b = parseFloat(item["ChannelB"]);
            conf.lux_coeff_lir[i].c = parseFloat(item["ChannelC"]);
        }
    }

    // Parse LuxCoeffHIR
    if (doc.isMember("LuxCoeffHIR")) {
        const Json::Value& arr = doc["LuxCoeffHIR"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 3; i++) {
            const Json::Value& item = arr[i];
            conf.lux_coeff_hir[i].r = parseFloat(item["ChannelR"]);
            conf.lux_coeff_hir[i].g = parseFloat(item["ChannelG"]);
            conf.lux_coeff_hir[i].b = parseFloat(item["ChannelB"]);
            conf.lux_coeff_hir[i].c = parseFloat(item["ChannelC"]);
        }
    }

    // Parse LuxCoeffSuperHIR
    if (doc.isMember("LuxCoeffSuperHIR")) {
        const Json::Value& arr = doc["LuxCoeffSuperHIR"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 3; i++) {
            const Json::Value& item = arr[i];
            conf.lux_coeff_super_hir[i].r = parseFloat(item["ChannelR"]);
            conf.lux_coeff_super_hir[i].g = parseFloat(item["ChannelG"]);
            conf.lux_coeff_super_hir[i].b = parseFloat(item["ChannelB"]);
            conf.lux_coeff_super_hir[i].c = parseFloat(item["ChannelC"]);
        }
    }

    // Parse LinearityType
    if (doc.isMember("LinearityType")) {
        conf.linearity_type = doc["LinearityType"].asInt();
    }

    // Parse LinearityBrightnessRange
    if (doc.isMember("LinearityBrightnessRange")) {
        const Json::Value& arr = doc["LinearityBrightnessRange"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 9; i++) {
            const Json::Value& item = arr[i];
            conf.linearity_ranges[i].level = item["Level"].asInt();
            conf.linearity_ranges[i].bright_min = item["BrightnessMin"].asInt();
            conf.linearity_ranges[i].bright_max = item["BrightnessMax"].asInt();
        }
    }

    // Parse Linearity
    if (doc.isMember("Linearity")) {
        const Json::Value& arr = doc["Linearity"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 9; i++) {
            const Json::Value& item = arr[i];
            conf.linearity[i].function = item["Function"].asInt();

            if (item.isMember("LinearityParameter")) {
                const Json::Value& params = item["LinearityParameter"];
                for (Json::ArrayIndex ch = 0; ch < params.size() && ch < 4; ch++) {
                    const Json::Value& p = params[ch];
                    conf.linearity[i].channels[ch].p0 = parseFloat(p["Parameter0"]);
                    conf.linearity[i].channels[ch].p1 = parseFloat(p["Parameter1"]);
                    conf.linearity[i].channels[ch].p2 = parseFloat(p["Parameter2"]);
                    conf.linearity[i].channels[ch].p3 = parseFloat(p["Parameter3"]);
                }
            }
        }
    }

    // Parse Golden
    if (doc.isMember("Golden")) {
        const Json::Value& arr = doc["Golden"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 4; i++) {
            const Json::Value& item = arr[i];
            conf.golden[i].channel = item["Channel"].asInt();
            conf.golden[i].r = parseInt(item["RGolden"]);
            conf.golden[i].g = parseInt(item["GGolden"]);
            conf.golden[i].b = parseInt(item["BGolden"]);
            conf.golden[i].w = parseInt(item["WGolden"]);
        }
    }

    // Parse GreyScale
    if (doc.isMember("GreyScale")) {
        const Json::Value& arr = doc["GreyScale"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 4; i++) {
            const Json::Value& item = arr[i];
            conf.grayscale[i].channel = item["Channel"].asInt();
            conf.grayscale[i].r = parseFloat(item["RGreyscale"]);
            conf.grayscale[i].g = parseFloat(item["GGreyscale"]);
            conf.grayscale[i].b = parseFloat(item["BGreyscale"]);
        }
    }

    // Check for L_ mode
    conf.has_l_mode = doc.isMember("L_LuxCoeffLIR");
    if (conf.has_l_mode) {
        const Json::Value& arr = doc["L_IRBrightness"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 3; i++) {
            const Json::Value& item = arr[i];
            conf.l_ir_brightness[i].level = item["Level"].asInt();
            conf.l_ir_brightness[i].bright_min = item["BrightnessMin"].asInt();
            conf.l_ir_brightness[i].bright_max = item["BrightnessMax"].asInt();
        }

        const Json::Value& lux_arr = doc["L_LuxCoeffLIR"];
        for (Json::ArrayIndex i = 0; i < lux_arr.size() && i < 3; i++) {
            const Json::Value& item = lux_arr[i];
            conf.l_lux_coeff_lir[i].r = parseFloat(item["ChannelR"]);
            conf.l_lux_coeff_lir[i].g = parseFloat(item["ChannelG"]);
            conf.l_lux_coeff_lir[i].b = parseFloat(item["ChannelB"]);
            conf.l_lux_coeff_lir[i].c = parseFloat(item["ChannelC"]);
        }
    }

    // Check for M_ mode
    conf.has_m_mode = doc.isMember("M_LuxCoeffLIR");
    if (conf.has_m_mode) {
        const Json::Value& arr = doc["M_IRBrightness"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 3; i++) {
            const Json::Value& item = arr[i];
            conf.m_ir_brightness[i].level = item["Level"].asInt();
            conf.m_ir_brightness[i].bright_min = item["BrightnessMin"].asInt();
            conf.m_ir_brightness[i].bright_max = item["BrightnessMax"].asInt();
        }

        const Json::Value& lux_arr = doc["M_LuxCoeffLIR"];
        for (Json::ArrayIndex i = 0; i < lux_arr.size() && i < 3; i++) {
            const Json::Value& item = lux_arr[i];
            conf.m_lux_coeff_lir[i].r = parseFloat(item["ChannelR"]);
            conf.m_lux_coeff_lir[i].g = parseFloat(item["ChannelG"]);
            conf.m_lux_coeff_lir[i].b = parseFloat(item["ChannelB"]);
            conf.m_lux_coeff_lir[i].c = parseFloat(item["ChannelC"]);
        }
    }

    ALOGI("FusionLight config loaded: brightness_max=%d, has_l_mode=%d, has_m_mode=%d",
          conf.common.brightness_max, conf.has_l_mode, conf.has_m_mode);
    ALOGI("Screen location: (%d,%d) to (%d,%d)",
          conf.common.screenshot_rect.left_top_x, conf.common.screenshot_rect.left_top_y,
          conf.common.screenshot_rect.right_bottom_x, conf.common.screenshot_rect.right_bottom_y);

    return true;
}

static float applyLinearityCorrection(float raw_value, int brightness, int channel) {
    // Find appropriate linearity function based on brightness
    int func_idx = -1;
    for (int i = 0; i < 9; i++) {
        if (brightness >= conf.linearity_ranges[i].bright_min &&
            brightness <= conf.linearity_ranges[i].bright_max) {
            func_idx = i;
            break;
        }
    }

    if (func_idx < 0) return raw_value;

    // Apply cubic polynomial: p3*x³ + p2*x² + p1*x + p0
    const LinearityParams& params = conf.linearity[func_idx].channels[channel];
    float x = raw_value;
    float corrected = params.p3 * x * x * x +
                     params.p2 * x * x +
                     params.p1 * x +
                     params.p0;

    return corrected;
}

static int determineIRLevel(float brightness, float ir_ratio, const IRBrightness* ir_bright) {
    // Determine brightness range
    int brightness_level = -1;
    for (int i = 0; i < 3; i++) {
        if (brightness >= ir_bright[i].bright_min &&
            brightness <= ir_bright[i].bright_max) {
            brightness_level = i;
            break;
        }
    }

    // Verify with IR ratio
    for (int i = 0; i < 3; i++) {
        if (ir_ratio >= conf.ir_thresholds[i].ir_min &&
            ir_ratio < conf.ir_thresholds[i].ir_max) {
            return i;
        }
    }

    return brightness_level >= 0 ? brightness_level : 0;
}

void AlsCorrection::init() {
    if (!loadFusionLightConfig()) {
        ALOGE("Failed to load fusionlight.json config");
        return;
    }

    float calib_factor = 1.0f;

    for (auto& range : hysteresis_ranges) {
        range.min *= calib_factor;
        range.max *= calib_factor;
    }
    hysteresis_ranges[0].min = -1.0;

    ALOGI("ALS correction initialized with calib_factor=%.3f", calib_factor);

    // Initialize capture service
    const auto instancename = std::string(IAreaCapture::descriptor) + "/default";
    if (AServiceManager_isDeclared(instancename.c_str())) {
        service = IAreaCapture::fromBinder(::ndk::SpAIBinder(
            AServiceManager_waitForService(instancename.c_str())));
    } else {
        ALOGW("IAreaCapture service not registered, screen correction will be limited");
    }
}

void AlsCorrection::process(Event& event) {
    static AreaRgbCaptureResult screenshot = { 0.0, 0.0, 0.0 };

    ALOGV("Raw sensor reading: %.0f", event.u.scalar);

    nsecs_t now = systemTime(SYSTEM_TIME_BOOTTIME);
    float brightness = get(BRIGHTNESS_DIR "brightness", 0.0);
    int bright_level = static_cast<int>(brightness);

    if (state.last_update == 0) {
        state.last_update = now;
        state.last_forced_update = now;
    } else {
        if (brightness > 0.0 && (now - state.last_forced_update) > s2ns(3)) {
            ALOGV("Forcing screenshot");
            state.last_forced_update = now;
            state.force_update = true;
        }
        if ((now - state.last_update) < ms2ns(100)) {
            ALOGV("Events coming too fast, dropping");
            event.sensorHandle = 0;
            return;
        }
        state.last_update = now;
    }

    // Assume single clear channel like original code
    float raw_clear = event.u.scalar;

    // Apply linearity correction to clear channel (channel 3 = C/White)
    float corrected_clear = applyLinearityCorrection(raw_clear, bright_level, 3);

    ALOGV("Linearity corrected clear: %.2f", corrected_clear);

    // For IR ratio calculation, we need RGB values
    float ir_ratio = 0.5;  // Default if we can't calculate

    // Determine display mode based on brightness
    const IRBrightness* ir_bright = conf.ir_brightness;
    const LuxCoeff* lux_lir = conf.lux_coeff_lir;

    if (conf.has_l_mode && brightness <= 1105) {
        ir_bright = conf.l_ir_brightness;
        lux_lir = conf.l_lux_coeff_lir;
        ALOGV("Using L_ mode (low brightness)");
    } else if (conf.has_m_mode && brightness > 1105 && brightness <= 1246) {
        ir_bright = conf.m_ir_brightness;
        lux_lir = conf.m_lux_coeff_lir;
        ALOGV("Using M_ mode (medium brightness)");
    }

    // Determine IR level (use brightness-based for now since we don't have true IR ratio)
    int ir_level = 0;
    for (int i = 0; i < 3; i++) {
        if (brightness >= ir_bright[i].bright_min &&
            brightness <= ir_bright[i].bright_max) {
            ir_level = i;
            break;
        }
    }

    // Select appropriate lux coefficients
    // Without true IR ratio, default to LIR for indoor, HIR for outdoor brightness levels
    const LuxCoeff* lux_coeff;
    if (brightness < 2000) {
        lux_coeff = &lux_lir[ir_level];
        ALOGV("Using LIR coefficients (indoor brightness)");
    } else if (brightness < 3000) {
        lux_coeff = &conf.lux_coeff_hir[ir_level];
        ALOGV("Using HIR coefficients (mid brightness)");
    } else {
        lux_coeff = &conf.lux_coeff_super_hir[ir_level];
        ALOGV("Using SuperHIR coefficients (outdoor brightness)");
    }

    // Calculate lux from corrected clear channel
    // Since we don't have separate RGB, use clear channel with C coefficient
    float calculated_lux = corrected_clear * lux_coeff->c;

    ALOGV("IR level: %d, Calculated lux: %.2f", ir_level, calculated_lux);

    // Apply screen brightness correction if needed
    float final_lux = calculated_lux;
    float brightness_fullwhite = 0.0f;

    if (service != nullptr && brightness > 0.0 &&
        (state.force_update ||
         (calculated_lux < state.hyst_min || calculated_lux > state.hyst_max))) {

        if (service->getAreaBrightness(&screenshot).isOk()) {
            if (screenshot.r + screenshot.g + screenshot.b > 0) {
                ALOGV("Screen color above sensor: %.2f %.2f %.2f",
                      screenshot.r, screenshot.g, screenshot.b);

                auto srgb_to_linear = [](float x) -> float {
                    x = x / 255.0f;
                    return (x <= 0.04045f) ? (x / 12.92f) : std::pow((x + 0.055f) / 1.055f, 2.4f);
                };

                float lin_r = srgb_to_linear(screenshot.r);
                float lin_g = srgb_to_linear(screenshot.g);
                float lin_b = srgb_to_linear(screenshot.b);

                // Check if this is grey/white
                float rgb_delta = std::max({std::abs(screenshot.r - screenshot.g),
                                           std::abs(screenshot.g - screenshot.b),
                                           std::abs(screenshot.b - screenshot.r)});

                float luminance;
                if (conf.grayscale[3].r > 0 || conf.grayscale[3].g > 0 || conf.grayscale[3].b > 0) {
                    luminance = lin_r * conf.grayscale[3].r +
                              lin_g * conf.grayscale[3].g +
                              lin_b * conf.grayscale[3].b;
                } else {
                    luminance = lin_r * 0.2126f + lin_g * 0.7152f + lin_b * 0.0722f;
                }

                // Find appropriate CCT segment based on calculated_lux
                const CCTSegment* active_segment = nullptr;
                for (int i = 0; i < conf.cct_segment_count; i++) {
                    // For very low ambient (pitch black room), bias toward segment 0 threshold detection
                    // Use segment if calculated_lux is within 10 lux of min (helps catch edge cases)
                    int min_threshold = std::max(0, conf.cct_segments[i].lux_min - 10);
                    if (calculated_lux >= min_threshold && calculated_lux <= conf.cct_segments[i].lux_max) {
                        active_segment = &conf.cct_segments[i];
                        ALOGV("Selected CCT segment %d for lux %.2f (range %d-%d)",
                              i, calculated_lux, conf.cct_segments[i].lux_min, conf.cct_segments[i].lux_max);
                        break;
                    }
                }

                // Estimate screen brightness contribution
                float brightness_ratio = brightness / conf.common.brightness_max;
                float golden_scale = conf.golden[3].w / 1000.0f;
                float base_contrib = luminance * brightness_ratio * golden_scale;

                // Additional global boost for white pages in very dark rooms
                // This multiplier is applied on top of CCT segment boost
                constexpr float kDarkRoomWhiteBoost = 1.35f;  // +35% extra for pitch-black + white

                // Apply segment-specific leakage boost if color detected
                float leak_boost = 1.0f;
                if (active_segment != nullptr) {
                    // Check if this qualifies as GREY_SCREEN or WHITE_BLACK_SCREEN
                    for (int pc_idx = 0; pc_idx < active_segment->pure_color_count; pc_idx++) {
                        const auto& pc = active_segment->pure_colors[pc_idx];
                        // Match grey/white screens
                        if ((pc.color_name == "GREY_SCREEN" || pc.color_name == "GREYSCREEN" ||
                             pc.color_name == "WHITE_BLACK_SCREEN" || pc.color_name == "WHITEBLACKSCREEN") &&
                            rgb_delta <= pc.grey_scale_delta_threshold) {
                            // Calculate leak ratio proxy: luminance * brightness as fraction of max leak
                            float leak_proxy = luminance * brightness_ratio;
                            float seg_leak_thresh = active_segment->leak_ratio_threshold;
                            float seg_leak_max = active_segment->max_leak_ratio_threshold;
                            // Lower threshold for dark room detection - be more aggressive
                            float dark_room_thresh = seg_leak_thresh * 0.75f;  // Trigger earlier

                            if (leak_proxy >= dark_room_thresh && pc.leak_ratio_max > 0.0f) {
                                // Map leak_proxy to boost
                                float t = (seg_leak_max > seg_leak_thresh) ?
                                        (leak_proxy - dark_room_thresh) / (seg_leak_max - dark_room_thresh) : 0.0f;
                                t = std::min(std::max(t, 0.0f), 1.0f);
                                leak_boost = 1.0f + t * (pc.leak_ratio_max - 1.0f);

                                // Apply extra dark room boost when ambient is very low and white detected
                                if (calculated_lux < 40.0f && luminance > 0.75f) {
                                    leak_boost *= kDarkRoomWhiteBoost;
                                    ALOGV("Applied dark room white boost (%.2f)", kDarkRoomWhiteBoost);
                                }

                                ALOGV("CCT leak boost: seg=%d, color=%s, rgb_delta=%.1f, leak_proxy=%.3f, boost=%.2f",
                                      active_segment->level, pc.color_name.c_str(), rgb_delta, leak_proxy, leak_boost);
                                break;
                            }
                        }
                    }
                } else {
                    // No segment matched - very low ambient, apply conservative fallback boost
                    if (calculated_lux < 30.0f && rgb_delta <= 36 && luminance > 0.75f) {
                        leak_boost = 1.5f * kDarkRoomWhiteBoost;
                        ALOGV("Fallback dark room boost applied: %.2f", leak_boost);
                    }
                }
                // Final screen contribution with leakage boost
                float screen_lux_contrib = base_contrib * leak_boost;
                brightness_fullwhite = brightness_ratio * golden_scale * leak_boost;

                // Ensure minimum subtraction in pitch black + white scenario
                if (calculated_lux < 50.0f && luminance > 0.70f) {
                    screen_lux_contrib = std::max(screen_lux_contrib, base_contrib * 1.8f);
                }

                // Subtract screen contribution
                final_lux = std::max(calculated_lux - screen_lux_contrib, 0.0f);

                ALOGV("Screen: base=%.2f, boost=%.2f, contrib=%.2f lux, fullwhite=%.2f, final=%.2f",
                      base_contrib, leak_boost, screen_lux_contrib, brightness_fullwhite, final_lux);
            } else {
                // Black screen, no correction needed
                ALOGV("Black screen, no correction");
            }

            // Update hysteresis range
            for (auto& range : hysteresis_ranges) {
                if (final_lux <= range.middle) {
                    state.hyst_min = range.min;
                    state.hyst_max = range.max + brightness_fullwhite;
                    break;
                }
            }

            state.force_update = false;
            state.last_corrected_value = final_lux;
        } else {
            ALOGV("Failed to get screen brightness, using calculated lux");
            state.last_corrected_value = calculated_lux;
        }
    } else if (calculated_lux >= state.hyst_min && calculated_lux <= state.hyst_max) {
        // Within hysteresis range, use cached value for stability
        final_lux = state.last_corrected_value;
        ALOGV("Within hysteresis (%.1f-%.1f), using cached: %.2f lux",
              state.hyst_min, state.hyst_max, final_lux);
    } else {
        // Outside hysteresis but no screen update, use calculated
        state.last_corrected_value = calculated_lux;
        final_lux = calculated_lux;
    }

    event.u.scalar = final_lux;
    ALOGV("Final corrected lux: %.0f", event.u.scalar);
}

}  // namespace implementation
}  // namespace V2_1
}  // namespace sensors
}  // namespace hardware
}  // namespace android
