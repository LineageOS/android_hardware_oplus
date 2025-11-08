/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-FileCopyrightText: 2025 The YAAP Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "AlsCorrection.h"

#include <fstream>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/stringprintf.h>
#include <android/binder_manager.h>
#include <json/json.h>

using aidl::vendor::lineage::oplus_als::AreaRgbCaptureResult;
using android::base::ReadFileToString;
using android::base::StringAppendF;
using android::base::StringPrintf;

constexpr auto kBrightnessPath = "/sys/class/backlight/panel0-backlight/brightness";
constexpr auto kFusionLightJson = "/odm/etc/fusionlight.json";

namespace android {
namespace hardware {
namespace sensors {
namespace V2_1 {
namespace subhal {
namespace implementation {
namespace wrapper {

namespace {

float ParseFloat(const Json::Value& val) {
    if (val.isString()) {
        return std::stof(val.asString());
    }
    return val.asFloat();
}

int ParseInt(const Json::Value& val) {
    if (val.isString()) {
        return std::stoi(val.asString());
    }
    return val.asInt();
}

}  // anonymous namespace

bool AlsCorrection::loadFusionLightConfig() {
    std::ifstream ifs(kFusionLightJson);
    if (!ifs.is_open()) {
        LOG(ERROR) << "Failed to open " << kFusionLightJson;
        return false;
    }

    Json::CharReaderBuilder rbuilder;
    rbuilder["collectComments"] = false;
    std::string errs;
    Json::Value doc;

    if (!Json::parseFromStream(rbuilder, ifs, &doc, &errs)) {
        LOG(ERROR) << "JSON parse error: " << errs;
        return false;
    }

    // Parse CommonConfig
    if (doc.isMember("CommonConfig")) {
        const Json::Value& cc = doc["CommonConfig"];
        if (cc.isMember("ScreenShotRect")) {
            const Json::Value& rect = cc["ScreenShotRect"];
            conf_.common.screenshot_rect.left_top_x = rect["LeftTopX"].asInt();
            conf_.common.screenshot_rect.left_top_y = rect["LeftTopY"].asInt();
            conf_.common.screenshot_rect.right_bottom_x = rect["RightBottomX"].asInt();
            conf_.common.screenshot_rect.right_bottom_y = rect["RightBottomY"].asInt();
        }
        conf_.common.brightness_max = cc.get("BrightnessMax", 4095).asInt();
        conf_.common.normal_brightness_max = cc.get("NormalModeBrightnessMax", 3332).asInt();
        conf_.common.fusion_rgb_supported = cc.get("FusionRGBSupported", true).asBool();
    }

    // Parse CCT leakage
    conf_.cct_segment_count = 0;
    if (doc.isMember("CCTSegmentRange") && doc.isMember("CCTSegmentPureColorParameter")) {
        const Json::Value& seg_range = doc["CCTSegmentRange"];
        const Json::Value& seg_pure = doc["CCTSegmentPureColorParameter"];

        for (Json::ArrayIndex seg_idx = 0; seg_idx < seg_range.size() && seg_idx < 5; seg_idx++) {
            const Json::Value& seg = seg_range[seg_idx];
            int level = seg["Level"].asInt();
            conf_.cct_segments[level].level = level;
            conf_.cct_segments[level].lux_min = seg["LuxMin"].asInt();
            conf_.cct_segments[level].lux_max = seg["LuxMax"].asInt();
            conf_.cct_segments[level].max_leak_ratio_threshold =
                    ParseFloat(seg["CCTMaxLeakRatioThreshold"]);
            conf_.cct_segments[level].leak_ratio_threshold =
                    ParseFloat(seg["CCTLeakRatioThreshold"]);
            // Find matching pure color params by Level field (not array index!)
            conf_.cct_segments[level].pure_color_count = 0;
            for (Json::ArrayIndex pure_idx = 0; pure_idx < seg_pure.size(); pure_idx++) {
                if (seg_pure[pure_idx]["Level"].asInt() == level &&
                    seg_pure[pure_idx].isMember("CCTPureColorParameter")) {
                    const Json::Value& colors = seg_pure[pure_idx]["CCTPureColorParameter"];
                    for (Json::ArrayIndex color_idx = 0; color_idx < colors.size() && color_idx < 9;
                         color_idx++) {
                        const Json::Value& col = colors[color_idx];
                        auto& pc = conf_.cct_segments[level].pure_colors[color_idx];
                        pc.color_id = col["PureColorId"].asInt();
                        pc.color_name = col["PureColorName"].asString();
                        pc.grey_scale_delta_threshold = col["GreyScaleDeltaThreshold"].asInt();
                        pc.leak_ratio_min = ParseFloat(col["LeakRatioMin"]);
                        pc.leak_ratio_max = ParseFloat(col["LeakRatioMax"]);
                        conf_.cct_segments[level].pure_color_count++;
                    }
                    break;
                }
            }
            conf_.cct_segment_count++;
        }
    }

    if (conf_.cct_segment_count > 0) {
        std::string info("Loaded " + std::to_string(conf_.cct_segment_count) +
                         " CCT leak segments:");
        for (int i = 0; i < conf_.cct_segment_count; i++) {
            StringAppendF(&info, "  Seg%d: lux %d-%d, leak thresh %.3f-%.3f, %d pure colors",
                          conf_.cct_segments[i].level, conf_.cct_segments[i].lux_min,
                          conf_.cct_segments[i].lux_max, conf_.cct_segments[i].leak_ratio_threshold,
                          conf_.cct_segments[i].max_leak_ratio_threshold,
                          conf_.cct_segments[i].pure_color_count);
        }
        LOG(INFO) << info;
    }

    // Parse IRThreshold
    if (doc.isMember("IRThreshold")) {
        const Json::Value& arr = doc["IRThreshold"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 3; i++) {
            const Json::Value& item = arr[i];
            conf_.ir_thresholds[i].level = item["Level"].asInt();
            conf_.ir_thresholds[i].ir_min = ParseFloat(item["IR_Ratio_Min"]);
            conf_.ir_thresholds[i].ir_max = ParseFloat(item["IR_Ratio_Max"]);
        }
    }

    // Parse IRBrightness
    if (doc.isMember("IRBrightness")) {
        const Json::Value& arr = doc["IRBrightness"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 3; i++) {
            const Json::Value& item = arr[i];
            conf_.ir_brightness[i].level = item["Level"].asInt();
            conf_.ir_brightness[i].bright_min = item["BrightnessMin"].asInt();
            conf_.ir_brightness[i].bright_max = item["BrightnessMax"].asInt();
        }
    }

    // Parse LuxCoeffLIR
    if (doc.isMember("LuxCoeffLIR")) {
        const Json::Value& arr = doc["LuxCoeffLIR"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 3; i++) {
            const Json::Value& item = arr[i];
            conf_.lux_coeff_lir[i].r = ParseFloat(item["ChannelR"]);
            conf_.lux_coeff_lir[i].g = ParseFloat(item["ChannelG"]);
            conf_.lux_coeff_lir[i].b = ParseFloat(item["ChannelB"]);
            conf_.lux_coeff_lir[i].c = ParseFloat(item["ChannelC"]);
        }
    }

    // Parse LuxCoeffHIR
    if (doc.isMember("LuxCoeffHIR")) {
        const Json::Value& arr = doc["LuxCoeffHIR"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 3; i++) {
            const Json::Value& item = arr[i];
            conf_.lux_coeff_hir[i].r = ParseFloat(item["ChannelR"]);
            conf_.lux_coeff_hir[i].g = ParseFloat(item["ChannelG"]);
            conf_.lux_coeff_hir[i].b = ParseFloat(item["ChannelB"]);
            conf_.lux_coeff_hir[i].c = ParseFloat(item["ChannelC"]);
        }
    }

    // Parse LuxCoeffSuperHIR
    if (doc.isMember("LuxCoeffSuperHIR")) {
        const Json::Value& arr = doc["LuxCoeffSuperHIR"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 3; i++) {
            const Json::Value& item = arr[i];
            conf_.lux_coeff_super_hir[i].r = ParseFloat(item["ChannelR"]);
            conf_.lux_coeff_super_hir[i].g = ParseFloat(item["ChannelG"]);
            conf_.lux_coeff_super_hir[i].b = ParseFloat(item["ChannelB"]);
            conf_.lux_coeff_super_hir[i].c = ParseFloat(item["ChannelC"]);
        }
    }

    // Parse LinearityType
    if (doc.isMember("LinearityType")) {
        conf_.linearity_type = doc["LinearityType"].asInt();
    }

    // Parse LinearityBrightnessRange
    if (doc.isMember("LinearityBrightnessRange")) {
        const Json::Value& arr = doc["LinearityBrightnessRange"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 9; i++) {
            const Json::Value& item = arr[i];
            conf_.linearity_ranges[i].level = item["Level"].asInt();
            conf_.linearity_ranges[i].bright_min = item["BrightnessMin"].asInt();
            conf_.linearity_ranges[i].bright_max = item["BrightnessMax"].asInt();
        }
    }

    // Parse Linearity
    if (doc.isMember("Linearity")) {
        const Json::Value& arr = doc["Linearity"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 9; i++) {
            const Json::Value& item = arr[i];
            conf_.linearity[i].function = item["Function"].asInt();

            if (item.isMember("LinearityParameter")) {
                const Json::Value& params = item["LinearityParameter"];
                for (Json::ArrayIndex ch = 0; ch < params.size() && ch < 4; ch++) {
                    const Json::Value& p = params[ch];
                    conf_.linearity[i].channels[ch].p0 = ParseFloat(p["Parameter0"]);
                    conf_.linearity[i].channels[ch].p1 = ParseFloat(p["Parameter1"]);
                    conf_.linearity[i].channels[ch].p2 = ParseFloat(p["Parameter2"]);
                    conf_.linearity[i].channels[ch].p3 = ParseFloat(p["Parameter3"]);
                }
            }
        }
    }

    // Parse Golden
    if (doc.isMember("Golden")) {
        const Json::Value& arr = doc["Golden"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 4; i++) {
            const Json::Value& item = arr[i];
            conf_.golden[i].channel = item["Channel"].asInt();
            conf_.golden[i].r = ParseInt(item["RGolden"]);
            conf_.golden[i].g = ParseInt(item["GGolden"]);
            conf_.golden[i].b = ParseInt(item["BGolden"]);
            conf_.golden[i].w = ParseInt(item["WGolden"]);
        }
    }

    // Parse GreyScale
    if (doc.isMember("GreyScale")) {
        const Json::Value& arr = doc["GreyScale"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 4; i++) {
            const Json::Value& item = arr[i];
            conf_.grayscale[i].channel = item["Channel"].asInt();
            conf_.grayscale[i].r = ParseFloat(item["RGreyscale"]);
            conf_.grayscale[i].g = ParseFloat(item["GGreyscale"]);
            conf_.grayscale[i].b = ParseFloat(item["BGreyscale"]);
        }
    }

    // Check for L_ mode
    conf_.has_l_mode = doc.isMember("L_LuxCoeffLIR");
    if (conf_.has_l_mode) {
        const Json::Value& arr = doc["L_IRBrightness"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 3; i++) {
            const Json::Value& item = arr[i];
            conf_.l_ir_brightness[i].level = item["Level"].asInt();
            conf_.l_ir_brightness[i].bright_min = item["BrightnessMin"].asInt();
            conf_.l_ir_brightness[i].bright_max = item["BrightnessMax"].asInt();
        }

        const Json::Value& lux_arr = doc["L_LuxCoeffLIR"];
        for (Json::ArrayIndex i = 0; i < lux_arr.size() && i < 3; i++) {
            const Json::Value& item = lux_arr[i];
            conf_.l_lux_coeff_lir[i].r = ParseFloat(item["ChannelR"]);
            conf_.l_lux_coeff_lir[i].g = ParseFloat(item["ChannelG"]);
            conf_.l_lux_coeff_lir[i].b = ParseFloat(item["ChannelB"]);
            conf_.l_lux_coeff_lir[i].c = ParseFloat(item["ChannelC"]);
        }
    }

    // Check for M_ mode
    conf_.has_m_mode = doc.isMember("M_LuxCoeffLIR");
    if (conf_.has_m_mode) {
        const Json::Value& arr = doc["M_IRBrightness"];
        for (Json::ArrayIndex i = 0; i < arr.size() && i < 3; i++) {
            const Json::Value& item = arr[i];
            conf_.m_ir_brightness[i].level = item["Level"].asInt();
            conf_.m_ir_brightness[i].bright_min = item["BrightnessMin"].asInt();
            conf_.m_ir_brightness[i].bright_max = item["BrightnessMax"].asInt();
        }

        const Json::Value& lux_arr = doc["M_LuxCoeffLIR"];
        for (Json::ArrayIndex i = 0; i < lux_arr.size() && i < 3; i++) {
            const Json::Value& item = lux_arr[i];
            conf_.m_lux_coeff_lir[i].r = ParseFloat(item["ChannelR"]);
            conf_.m_lux_coeff_lir[i].g = ParseFloat(item["ChannelG"]);
            conf_.m_lux_coeff_lir[i].b = ParseFloat(item["ChannelB"]);
            conf_.m_lux_coeff_lir[i].c = ParseFloat(item["ChannelC"]);
        }
    }

    LOG(INFO) << StringPrintf(
            "FusionLight config loaded: brightness_max=%d, has_l_mode=%d, has_m_mode=%d",
            conf_.common.brightness_max, conf_.has_l_mode, conf_.has_m_mode);
    LOG(INFO) << StringPrintf(
            "Screen location: (%d,%d) to (%d,%d)", conf_.common.screenshot_rect.left_top_x,
            conf_.common.screenshot_rect.left_top_y, conf_.common.screenshot_rect.right_bottom_x,
            conf_.common.screenshot_rect.right_bottom_y);

    return true;
}

float AlsCorrection::applyLinearityCorrection(float raw_value, int brightness, int channel) {
    // Find appropriate linearity function based on brightness
    int func_idx = -1;
    for (int i = 0; i < 9; i++) {
        if (brightness >= conf_.linearity_ranges[i].bright_min &&
            brightness <= conf_.linearity_ranges[i].bright_max) {
            func_idx = i;
            break;
        }
    }

    if (func_idx < 0) return raw_value;

    // Apply cubic polynomial: p3*x³ + p2*x² + p1*x + p0
    const LinearityParams& params = conf_.linearity[func_idx].channels[channel];
    float x = raw_value;
    float corrected = params.p3 * x * x * x + params.p2 * x * x + params.p1 * x + params.p0;

    return corrected;
}

int AlsCorrection::determineIRLevel(float brightness, float ir_ratio,
                                    const IRBrightness* ir_bright) {
    // Determine brightness range
    int brightness_level = -1;
    for (int i = 0; i < 3; i++) {
        if (brightness >= ir_bright[i].bright_min && brightness <= ir_bright[i].bright_max) {
            brightness_level = i;
            break;
        }
    }

    // Verify with IR ratio
    for (int i = 0; i < 3; i++) {
        if (ir_ratio >= conf_.ir_thresholds[i].ir_min && ir_ratio < conf_.ir_thresholds[i].ir_max) {
            return i;
        }
    }

    return brightness_level >= 0 ? brightness_level : 0;
}

bool AlsCorrection::init() {
    if (!loadFusionLightConfig()) {
        LOG(ERROR) << "Failed to load fusionlight.json config";
        return false;
    }

    float calib_factor = 1.0f;

    for (auto& range : hysteresis_ranges_) {
        range.min *= calib_factor;
        range.max *= calib_factor;
    }
    hysteresis_ranges_[0].min = -1.0;

    LOG(INFO) << "ALS correction initialized with calib_factor=" << calib_factor;

    // Initialize capture service
    const auto instancename = std::string(IAreaCapture::descriptor) + "/default";
    if (AServiceManager_isDeclared(instancename.c_str())) {
        service_ = IAreaCapture::fromBinder(
                ::ndk::SpAIBinder(AServiceManager_waitForService(instancename.c_str())));
    } else {
        LOG(WARNING) << "IAreaCapture service not registered, screen correction will be limited";
    }

    return true;
}

float AlsCorrection::process(const Event& event) {
    LOG(VERBOSE) << "Raw sensor reading: " << event.u.scalar;

    std::string buf;
    int brightness = 0;
    if (ReadFileToString(kBrightnessPath, &buf, true))
        brightness = std::stoi(buf);
    else
        LOG(ERROR) << "Failed to read screen brightness, assuming 0";

    nsecs_t now = systemTime(SYSTEM_TIME_BOOTTIME);
    if (last_update_ == 0) {
        last_update_ = now;
        last_forced_update_ = now;
        force_update_ = true;
    } else {
        if (brightness > 0.0 && (now - last_forced_update_) > s2ns(3)) {
            LOG(VERBOSE) << "Forcing screenshot";
            last_forced_update_ = now;
            force_update_ = true;
        }
        if ((now - last_update_) < ms2ns(100)) {
            LOG(WARNING) << "Events coming too fast, dropping";
            return -1.f;
        }
        last_update_ = now;
    }

    // Assume single clear channel like original code
    float raw_clear = event.u.scalar;

    // Apply linearity correction to clear channel (channel 3 = C/White)
    float corrected_clear = applyLinearityCorrection(raw_clear, brightness, 3);

    LOG(VERBOSE) << "Linearity corrected clear: " << corrected_clear;

    // For IR ratio calculation, we need RGB values
    float ir_ratio = 0.5;  // Default if we can't calculate

    // Determine display mode based on brightness
    const IRBrightness* ir_bright = conf_.ir_brightness;
    const LuxCoeff* lux_lir = conf_.lux_coeff_lir;

    if (conf_.has_l_mode && brightness <= 1105) {
        ir_bright = conf_.l_ir_brightness;
        lux_lir = conf_.l_lux_coeff_lir;
        LOG(VERBOSE) << "Using L_ mode (low brightness)";
    } else if (conf_.has_m_mode && brightness > 1105 && brightness <= 1246) {
        ir_bright = conf_.m_ir_brightness;
        lux_lir = conf_.m_lux_coeff_lir;
        LOG(VERBOSE) << "Using M_ mode (medium brightness)";
    }

    // Determine IR level (use brightness-based for now since we don't have true IR ratio)
    int ir_level = 0;
    for (int i = 0; i < 3; i++) {
        if (brightness >= ir_bright[i].bright_min && brightness <= ir_bright[i].bright_max) {
            ir_level = i;
            break;
        }
    }

    // Select appropriate lux coefficients
    // Without true IR ratio, default to LIR for indoor, HIR for outdoor brightness levels
    const LuxCoeff* lux_coeff;
    if (brightness < 2000) {
        lux_coeff = &lux_lir[ir_level];
        LOG(VERBOSE) << "Using LIR coefficients (indoor brightness)";
    } else if (brightness < 3000) {
        lux_coeff = &conf_.lux_coeff_hir[ir_level];
        LOG(VERBOSE) << "Using HIR coefficients (mid brightness)";
    } else {
        lux_coeff = &conf_.lux_coeff_super_hir[ir_level];
        LOG(VERBOSE) << "Using SuperHIR coefficients (outdoor brightness)";
    }

    // Calculate lux from corrected clear channel
    // Since we don't have separate RGB, use clear channel with C coefficient
    float calculated_lux = corrected_clear * lux_coeff->c;

    LOG(VERBOSE) << "IR level: " << ir_level << ", Calculated lux: " << calculated_lux;

    // Apply screen brightness correction if needed
    float final_lux = calculated_lux;
    float brightness_fullwhite = 0.0f;

    if (service_ != nullptr && brightness > 0 &&
        (force_update_ || (calculated_lux < hyst_min_ || calculated_lux > hyst_max_))) {
        AreaRgbCaptureResult screenshot;
        if (service_->getAreaBrightness(&screenshot).isOk()) {
            if (screenshot.r + screenshot.g + screenshot.b > 0) {
                LOG(VERBOSE) << "Screen color above sensor: " << screenshot.r << ", "
                             << screenshot.g << ", " << screenshot.b;

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
                if (conf_.grayscale[3].r > 0 || conf_.grayscale[3].g > 0 ||
                    conf_.grayscale[3].b > 0) {
                    luminance = lin_r * conf_.grayscale[3].r + lin_g * conf_.grayscale[3].g +
                                lin_b * conf_.grayscale[3].b;
                } else {
                    luminance = lin_r * 0.2126f + lin_g * 0.7152f + lin_b * 0.0722f;
                }

                // Find appropriate CCT segment based on calculated_lux
                const CCTSegment* active_segment = nullptr;
                for (int i = 0; i < conf_.cct_segment_count; i++) {
                    // For very low ambient (pitch black room), bias toward segment 0 threshold
                    // detection Use segment if calculated_lux is within 10 lux of min (helps
                    // catch edge cases)
                    int min_threshold = std::max(0, conf_.cct_segments[i].lux_min - 10);
                    if (calculated_lux >= min_threshold &&
                        calculated_lux <= conf_.cct_segments[i].lux_max) {
                        active_segment = &conf_.cct_segments[i];
                        LOG(VERBOSE) << StringPrintf(
                                "Selected CCT segment %d for lux %.2f (range %d-%d)", i,
                                calculated_lux, conf_.cct_segments[i].lux_min,
                                conf_.cct_segments[i].lux_max);
                        break;
                    }
                }

                // Estimate screen brightness contribution
                float brightness_ratio =
                        static_cast<float>(brightness) / conf_.common.brightness_max;
                float golden_scale = conf_.golden[3].w / 1000.0f;
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
                             pc.color_name == "WHITE_BLACK_SCREEN" ||
                             pc.color_name == "WHITEBLACKSCREEN") &&
                            rgb_delta <= pc.grey_scale_delta_threshold) {
                            // Calculate leak ratio proxy: luminance * brightness as fraction of
                            // max leak
                            float leak_proxy = luminance * brightness_ratio;
                            float seg_leak_thresh = active_segment->leak_ratio_threshold;
                            float seg_leak_max = active_segment->max_leak_ratio_threshold;
                            // Lower threshold for dark room detection - be more aggressive
                            float dark_room_thresh = seg_leak_thresh * 0.75f;  // Trigger earlier

                            if (leak_proxy >= dark_room_thresh && pc.leak_ratio_max > 0.0f) {
                                // Map leak_proxy to boost
                                float t = (seg_leak_max > seg_leak_thresh)
                                                  ? (leak_proxy - dark_room_thresh) /
                                                            (seg_leak_max - dark_room_thresh)
                                                  : 0.0f;
                                t = std::min(std::max(t, 0.0f), 1.0f);
                                leak_boost = 1.0f + t * (pc.leak_ratio_max - 1.0f);

                                // Apply extra dark room boost when ambient is very low and
                                // white detected
                                if (calculated_lux < 40.0f && luminance > 0.75f) {
                                    leak_boost *= kDarkRoomWhiteBoost;
                                    LOG(VERBOSE) << "Applied dark room white boost: "
                                                 << kDarkRoomWhiteBoost;
                                }

                                LOG(VERBOSE) << StringPrintf(
                                        "CCT leak boost: seg=%d, color=%s, rgb_delta=%.1f, "
                                        "leak_proxy=%.3f, boost=%.2f",
                                        active_segment->level, pc.color_name.c_str(), rgb_delta,
                                        leak_proxy, leak_boost);
                                break;
                            }
                        }
                    }
                } else {
                    // No segment matched - very low ambient, apply conservative fallback boost
                    if (calculated_lux < 30.0f && rgb_delta <= 36 && luminance > 0.75f) {
                        leak_boost = 1.5f * kDarkRoomWhiteBoost;
                        LOG(VERBOSE) << "Fallback dark room boost applied: " << leak_boost;
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

                LOG(VERBOSE) << StringPrintf(
                        "Screen: base=%.2f, boost=%.2f, contrib=%.2f lux, fullwhite=%.2f, "
                        "final=%.2f",
                        base_contrib, leak_boost, screen_lux_contrib, brightness_fullwhite,
                        final_lux);
            } else {
                // Black screen, no correction needed
                LOG(VERBOSE) << "Black screen, no correction";
            }

            // Update hysteresis range
            for (auto& range : hysteresis_ranges_) {
                if (final_lux <= range.middle) {
                    hyst_min_ = range.min;
                    hyst_max_ = range.max + brightness_fullwhite;
                    break;
                }
            }

            force_update_ = false;
            last_corrected_value_ = final_lux;
        } else {
            LOG(VERBOSE) << "Failed to get screen brightness, using calculated lux";
            last_corrected_value_ = calculated_lux;
        }
    } else if (calculated_lux >= hyst_min_ && calculated_lux <= hyst_max_) {
        // Within hysteresis range, use cached value for stability
        final_lux = last_corrected_value_;
        LOG(VERBOSE) << StringPrintf("Within hysteresis (%.1f-%.1f), using cached: %.2f lux",
                                     hyst_min_, hyst_max_, final_lux);
    } else {
        // Outside hysteresis but no screen update, use calculated
        last_corrected_value_ = calculated_lux;
        final_lux = calculated_lux;
    }

    LOG(VERBOSE) << "Final corrected lux: " << final_lux;
    return final_lux;
}

}  // namespace wrapper
}  // namespace implementation
}  // namespace subhal
}  // namespace V2_1
}  // namespace sensors
}  // namespace hardware
}  // namespace android
