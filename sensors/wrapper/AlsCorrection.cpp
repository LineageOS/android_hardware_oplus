/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-FileCopyrightText: 2025 The YAAP Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "AlsCorrection.h"

#include <algorithm>
#include <fstream>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/stringprintf.h>
#include <android/binder_manager.h>
#include <json/json.h>
#include <utils/Timers.h>

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

std::vector<BrightnessRange> ParseBrightnessRanges(const Json::Value& arr) {
    std::vector<BrightnessRange> out;
    out.resize(arr.size());
    for (const Json::Value& item : arr) {
        int level = item["Level"].asInt();
        out[level] = {item["BrightnessMin"].asInt(), item["BrightnessMax"].asInt()};
    }
    return out;
}

std::vector<std::vector<LuxCoeff>> ParseCctCoeff(const Json::Value& arr) {
    std::vector<std::vector<LuxCoeff>> out;

    if (arr.empty()) return out;

    int max_level = 0;
    for (const Json::Value& item : arr) {
        max_level = std::max(max_level, item["Level"].asInt());
    }
    out.resize(max_level + 1);

    for (auto& level_vec : out) {
        level_vec.resize(3);
    }

    // Parse items with indexing
    for (const Json::Value& item : arr) {
        int level = item["Level"].asInt();
        int component = item["Component"].asInt();
        if (level < static_cast<int>(out.size()) && component < 3) {
            out[level][component] = {
                ParseFloat(item["ChannelR"]),
                ParseFloat(item["ChannelG"]),
                ParseFloat(item["ChannelB"]),
                ParseFloat(item["ChannelC"])
            };
        }
    }

    return out;
}

std::vector<Golden> ParseGolden(const Json::Value& arr) {
    std::vector<Golden> out;
    out.resize(arr.size());
    for (const Json::Value& item : arr) {
        int ch = item["Channel"].asInt();
        out[ch] = {ParseInt(item["RGolden"]), ParseInt(item["GGolden"]), ParseInt(item["BGolden"]),
                   ParseInt(item["WGolden"])};
    }
    return out;
}

std::vector<GreyScale> ParseGreyScale(const Json::Value& arr) {
    std::vector<GreyScale> out;
    out.resize(arr.size());
    for (const Json::Value& item : arr) {
        int ch = item["Channel"].asInt();
        out[ch] = {ParseFloat(item["RGreyscale"]), ParseFloat(item["GGreyscale"]),
                   ParseFloat(item["BGreyscale"])};
    }
    return out;
}

float SrgbToLinear(float x) {
    x = x / 255.0f;
    return (x <= 0.04045f) ? (x / 12.92f) : std::pow((x + 0.055f) / 1.055f, 2.4f);
};

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
    {
        const Json::Value& cc = doc["CommonConfig"];
        const Json::Value& rect = cc["ScreenShotRect"];
        conf_.common = {{rect["LeftTopX"].asInt(), rect["LeftTopY"].asInt(),
                         rect["RightBottomX"].asInt(), rect["RightBottomY"].asInt()},
                        cc.get("BrightnessMax", 4095).asInt(),
                        cc.get("NormalModeBrightnessMax", 3332).asInt(),
                        cc.get("FusionRGBSupported", true).asBool()};
    }

    // Parse CCT leakage
    {
        const Json::Value& arr = doc["CCTSegmentRange"];
        conf_.cct_segments.resize(arr.size());
        for (const Json::Value& item : arr) {
            int level = item["Level"].asInt();
            conf_.cct_segments[level] = {item["LuxMin"].asInt(), item["LuxMax"].asInt(),
                                         item["CCTMaxLeakRatioThreshold"].asFloat(),
                                         item["CCTLeakRatioThreshold"].asFloat()};
        }
    }

    // Parse pure color params
    {
        const Json::Value& arr = doc["CCTSegmentPureColorParameter"];
        for (const Json::Value& item : arr) {
            int level = item["Level"].asInt();
            for (const Json::Value& col : item["CCTPureColorParameter"]) {
                auto name = col["PureColorName"].asString();
                if (name == "SPECIAL_PICTURE") {
                    continue;
                }
                conf_.cct_segments[level].pure_colors.emplace_back(
                        col["PureColorId"].asInt(), std::move(name),
                        col["GreyScaleDeltaThreshold"].asInt(), col["LeakRatioMin"].asFloat(),
                        col["LeakRatioMax"].asFloat());
            }
        }
    }

    std::string info("Loaded " + std::to_string(conf_.cct_segments.size()) + " CCT leak segments:");
    for (const auto& seg : conf_.cct_segments) {
        StringAppendF(&info, "  Seg: lux %d-%d, leak thresh %.3f-%.3f, %zu pure colors",
                      seg.lux_min, seg.lux_max, seg.leak_ratio_threshold,
                      seg.max_leak_ratio_threshold, seg.pure_colors.size());
    }
    LOG(INFO) << info;

    // Parse IRThreshold
    {
        const Json::Value& arr =
                doc.isMember("IRThreshold") ? doc["IRThreshold"] : doc["IRThreshold_V2_1"];
        conf_.ir_thresholds.resize(arr.size());
        for (const Json::Value& item : arr) {
            int level = item["Level"].asInt();
            conf_.ir_thresholds[level] = {item["IR_Ratio_Min"].asFloat(),
                                          item["IR_Ratio_Max"].asFloat()};
        }
    }

    // Parse IRBrightness
    {
        auto res = ParseBrightnessRanges(doc.isMember("IRBrightness") ? doc["IRBrightness"]
                                                                      : doc["IRBrightness_V2_1"]);
        std::move(res.begin(), res.end(), std::back_inserter(conf_.ir_brightness));
    }

    // Parse CCT coefficients
    {
        if (doc.isMember("CCTCoeffLIR")) {
            conf_.cct_coeff_lir = ParseCctCoeff(doc["CCTCoeffLIR"]);
        } else if (doc.isMember("LCCTCoeffLIR")) {
            conf_.cct_coeff_lir = ParseCctCoeff(doc["LCCTCoeffLIR"]);
        } else {
            conf_.cct_coeff_lir = ParseCctCoeff(Json::Value());
        }
    }

    {
        if (doc.isMember("CCTCoeffHIR")) {
            conf_.cct_coeff_hir = ParseCctCoeff(doc["CCTCoeffHIR"]);
        } else if (doc.isMember("MCCTCoeffHIR")) {
            conf_.cct_coeff_hir = ParseCctCoeff(doc["MCCTCoeffHIR"]);
        } else {
            conf_.cct_coeff_hir = ParseCctCoeff(Json::Value());
        }
    }

    {
        conf_.cct_coeff_super_hir =
                ParseCctCoeff(doc.get("CCTCoeffSuperHIR", Json::Value()));
    }

    // Parse LinearityBrightnessRange
    {
        auto res = ParseBrightnessRanges(doc["LinearityBrightnessRange"]);
        std::move(res.begin(), res.end(), std::back_inserter(conf_.linearity_ranges));
    }

    // Parse Linearity
    {
        if (doc.isMember("Linearity")) {
            const Json::Value& arr = doc["Linearity"];
            conf_.linearity.resize(arr.size());
            for (const Json::Value& item : arr) {
                int function = item["Function"].asInt();
                for (const Json::Value& p : item["LinearityParameter"]) {
                    int ch = p["Channel"].asInt();
                    conf_.linearity[function].channels[ch] = {
                            ParseFloat(p["Parameter0"]), ParseFloat(p["Parameter1"]),
                            ParseFloat(p["Parameter2"]), ParseFloat(p["Parameter3"])};
                };
            }
        } else if (doc.isMember("LinearityCompensation")) {
            const Json::Value& arr = doc["LinearityCompensation"];
            for (const Json::Value& item : arr) {
                int ch = item["channel"].asInt();
                const Json::Value& params = item["Parameter"];
                conf_.linearity.resize(params.size());
                for (const Json::Value& p : params) {
                    int level = p["level"].asInt();
                    conf_.linearity[level].channels[ch] = {
                            ParseFloat(p["Parameter0"]), ParseFloat(p["Parameter1"]),
                            ParseFloat(p["Parameter2"]), ParseFloat(p["Parameter3"])};
                }
            }
        } else {
            LOG(ERROR) << "No Linearity or LinearityCompensation found in config";
            return false;
        }

        if (conf_.linearity_ranges.size() != conf_.linearity.size()) {
            LOG(ERROR) << "Linearity size and range mismatch";
            return false;
        }
    }

    // Parse Golden
    {
        if (doc.isMember("Golden")) {
            auto res = ParseGolden(doc["Golden"]);
            std::move(res.begin(), res.end(), std::back_inserter(conf_.golden));
        } else if (doc.isMember("LightLeakageGolden")) {
            const Json::Value& arr = doc["LightLeakageGolden"];
            if (conf_.linearity_ranges.size() != arr.size()) {
                LOG(ERROR) << "LightLeakageGolden size and range mismatch";
                return false;
            }
            conf_.light_leakage_golden.resize(arr.size());
            for (const Json::Value& item : arr) {
                int level = item["level"].asInt();
                conf_.light_leakage_golden[level] = {
                        ParseInt(item["RGolden"]), ParseInt(item["GGolden"]),
                        ParseInt(item["BGolden"]), ParseInt(item["CGolden"])};
            }
        }

        // WIP: Parse L_ and M_ variants for mode-specific golden values
        if (doc.isMember("L_Golden")) {
            auto res = ParseGolden(doc["L_Golden"]);
            std::move(res.begin(), res.end(), std::back_inserter(conf_.golden));
        }
        if (doc.isMember("M_Golden")) {
            auto res = ParseGolden(doc["M_Golden"]);
            std::move(res.begin(), res.end(), std::back_inserter(conf_.golden));
        }
    }

    // Parse GreyScale
    {
        const Json::Value& arr = doc["GreyScale"];
        if (!arr.isNull()) {
            auto res = ParseGreyScale(arr);
            std::move(res.begin(), res.end(), std::back_inserter(conf_.greyscale));
        }
    }

    LOG(INFO) << StringPrintf(
            "FusionLight config loaded: brightness_max=%d, Screenshot location: (%d,%d) to (%d,%d)",
            conf_.common.brightness_max, conf_.common.screenshot_rect.left_top_x,
            conf_.common.screenshot_rect.left_top_y, conf_.common.screenshot_rect.right_bottom_x,
            conf_.common.screenshot_rect.right_bottom_y);

    return true;
}

float AlsCorrection::applyLinearityCorrection(float x, int linearity_level, int channel) {
    // Apply cubic polynomial: p3*x³ + p2*x² + p1*x + p0
    const LinearityParams& params = conf_.linearity[linearity_level].channels[channel];
    return params.p3 * x * x * x + params.p2 * x * x + params.p1 * x + params.p0;
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

    if (last_update_ == 0) {
        last_update_ = event.timestamp;
        last_forced_update_ = event.timestamp;
        force_update_ = true;
    } else {
        if (brightness > 0 && (event.timestamp - last_forced_update_) > s2ns(3)) {
            LOG(VERBOSE) << "Forcing screenshot";
            last_forced_update_ = event.timestamp;
            force_update_ = true;
        }
        if ((event.timestamp - last_update_) < ms2ns(100)) {
            LOG(WARNING) << "Events coming too fast, dropping";
            return -1.f;
        }
        last_update_ = event.timestamp;
    }

    float raw_clear = event.u.scalar;

    // Find appropriate linearity function based on brightness
    auto it_lin_range = std::find_if(
            conf_.linearity_ranges.begin(), conf_.linearity_ranges.end(),
            [&](const auto& r) { return brightness >= r.bright_min && brightness < r.bright_max; });
    if (it_lin_range == conf_.linearity_ranges.end()) {
        it_lin_range = std::find_if(
                conf_.linearity_ranges.begin(), conf_.linearity_ranges.end(), [&](const auto& r) {
                    return brightness >= r.bright_min && brightness <= r.bright_max;
                });
        if (it_lin_range == conf_.linearity_ranges.end()) {
            return raw_clear;
        }
    };
    int linearity_level = std::distance(conf_.linearity_ranges.begin(), it_lin_range);

    // Apply linearity correction to clear channel (channel 3 = C/White)
    float corrected_clear = applyLinearityCorrection(raw_clear, linearity_level, 3);
    LOG(VERBOSE) << "Linearity corrected clear: " << corrected_clear;

    // Determine IR level based on brightness
    auto it_ir_range = std::find_if(
            conf_.ir_brightness.begin(), conf_.ir_brightness.end(),
            [&](const auto& r) { return brightness >= r.bright_min && brightness < r.bright_max; });
    if (it_ir_range == conf_.ir_brightness.end()) {
        return raw_clear;
    }
    int ir_level = std::distance(conf_.ir_brightness.begin(), it_ir_range);

    /* Hardcode LIR for now. TODO: determine how to utilize HIR/SuperHIR
    const auto& cct_table = [&]() -> const std::vector<std::vector<LuxCoeff>>& {
        if (brightness < 2000) {
            LOG(VERBOSE) << "Using LIR coefficients (indoor brightness)";
            return conf_.cct_coeff_lir;
        } else if (brightness < 3000) {
            LOG(VERBOSE) << "Using HIR coefficients (mid brightness)";
            return conf_.cct_coeff_hir;
        } else {
            LOG(VERBOSE) << "Using SuperHIR coefficients (outdoor brightness)";
            return conf_.cct_coeff_super_hir;
        }
    }();
*/
    const auto& cct_table = conf_.cct_coeff_lir;

    // Calculate lux from corrected clear channel
    float coeff_c = 0.0f;
    if (!cct_table.empty()) {
        const int level = std::clamp(ir_level, 0, static_cast<int>(cct_table.size()) - 1);
        if (!cct_table[level].empty())
            coeff_c = cct_table[level][0].c;
    }
    float calculated_lux = corrected_clear * coeff_c;
    LOG(VERBOSE) << "IR level: " << ir_level << ", Lux coeff(c0): " << coeff_c
                 << ", Calculated lux: " << calculated_lux;

    // Apply screen brightness correction if needed
    float final_lux = calculated_lux;
    float screen_correction_amount = 0.0f;
    bool needs_screenshot = force_update_;
    
    // Trigger screenshot on significant lux change or forced update
    if (!needs_screenshot && service_ != nullptr && brightness > 0) {
        float lux_delta = std::abs(calculated_lux - last_calculated_lux_);
        if (lux_delta > 5.0f) {  // Only take screenshot on >5 lux change
            needs_screenshot = true;
            LOG(VERBOSE) << "Screenshot triggered by lux change: " << last_calculated_lux_ 
                         << " -> " << calculated_lux;
        }
    }

    if (service_ != nullptr && brightness > 0 && needs_screenshot) {
        AreaRgbCaptureResult screenshot;
        if (service_->getAreaBrightness(conf_.common.screenshot_rect.left_top_x,
                                        conf_.common.screenshot_rect.left_top_y,
                                        conf_.common.screenshot_rect.right_bottom_x,
                                        conf_.common.screenshot_rect.right_bottom_y, &screenshot)
                    .isOk()) {
            if (screenshot.r + screenshot.g + screenshot.b > 0) {
                LOG(VERBOSE) << "Screen color above sensor: " << screenshot.r << ", "
                             << screenshot.g << ", " << screenshot.b;

                float lin_r = SrgbToLinear(screenshot.r);
                float lin_g = SrgbToLinear(screenshot.g);
                float lin_b = SrgbToLinear(screenshot.b);

                // Check if this is grey/white
                float rgb_delta = std::max({std::abs(screenshot.r - screenshot.g),
                                            std::abs(screenshot.g - screenshot.b),
                                            std::abs(screenshot.b - screenshot.r)});

                // Calculate screen luminance contribution
                float screen_luminance;
                if (conf_.greyscale.size() > 3) {
                    screen_luminance = lin_r * conf_.greyscale[3].r +
                                       lin_g * conf_.greyscale[3].g +
                                       lin_b * conf_.greyscale[3].b;
                } else {
                    // Use standard Rec. 709 luminance
                    screen_luminance = lin_r * 0.2126f + lin_g * 0.7152f + lin_b * 0.0722f;
                }

                // Calculate screen-induced ambient light
                int golden_white = 0;
                if (!conf_.light_leakage_golden.empty() &&
                    linearity_level >= 0 &&
                    linearity_level < static_cast<int>(conf_.light_leakage_golden.size())) {
                    golden_white = conf_.light_leakage_golden[linearity_level].w;
                } else if (conf_.golden.size() > 3) {
                    golden_white = conf_.golden[3].w;
                } else {
                    LOG(WARNING) << "No usable Golden calibration present; disabling screen correction";
                    final_lux = calculated_lux;
                    last_corrected_value_ = final_lux;
                    last_calculated_lux_ = calculated_lux;
                    force_update_ = false;
                    return final_lux;
                }

                if (golden_white <= 0) {
                    LOG(WARNING) << "Invalid golden_white or coeff_c; disabling screen correction";
                    final_lux = calculated_lux;
                    last_corrected_value_ = final_lux;
                    last_calculated_lux_ = calculated_lux;
                    force_update_ = false;
                    return final_lux;
                }

                // Screen brightness as 0-1 ratio
                float brightness_ratio = static_cast<float>(brightness) / conf_.common.brightness_max;

                float leak_ratio = 1.0f;

                auto active_segment = std::find_if(
                        conf_.cct_segments.begin(), conf_.cct_segments.end(),
                        [&](const CCTSegment& seg) {
                            return calculated_lux >= seg.lux_min && calculated_lux <= seg.lux_max;
                        });

                if (active_segment != conf_.cct_segments.end()) {
                    for (const auto& pc : active_segment->pure_colors) {
                        if ((pc.color_name == "GREY_SCREEN" || pc.color_name == "GREYSCREEN" ||
                             pc.color_name == "WHITE_BLACK_SCREEN" || pc.color_name == "WHITEBLACKSCREEN") &&
                            rgb_delta <= pc.grey_scale_delta_threshold) {

                            // Map screen luminance to leak ratio boost
                            // Higher luminance = more light leak
                            float leak_ratio_min = pc.leak_ratio_min;
                            float leak_ratio_max = pc.leak_ratio_max;

                            // Interpolate based on screen luminance (0.0 = black, 1.0 = white)
                            leak_ratio = leak_ratio_min + screen_luminance * (leak_ratio_max - leak_ratio_min);
                            leak_ratio = std::clamp(leak_ratio, leak_ratio_min, leak_ratio_max);
                            break;
                        }
                    }
                }

                const float golden_lux = static_cast<float>(golden_white) * coeff_c;
                const float screen_base_lux = screen_luminance * brightness_ratio * golden_lux;
                screen_correction_amount = screen_base_lux * leak_ratio;
                // Clamp for not removing all ambient lux
                screen_correction_amount = std::min(screen_correction_amount, calculated_lux * 0.75f);
                
                final_lux = calculated_lux - screen_correction_amount;
                // Clamp to prevent negative values from noise/errors
                final_lux = std::max(0.5f, final_lux);

                LOG(VERBOSE) << StringPrintf(
                        "Screen correction: base_lux=%.2f, luminance=%.3f, leak_ratio=%.3f, "
                        "corrected=%.2f lux (%.0f%%), final=%.2f lux",
                        screen_base_lux, screen_luminance, leak_ratio,
                        screen_correction_amount, (screen_correction_amount / calculated_lux) * 100.0f, final_lux);
            } else {
                LOG(VERBOSE) << "Black screen detected, no correction needed";
                final_lux = calculated_lux;
            }

            // Update hysteresis range
            for (auto& range : hysteresis_ranges_) {
                if (calculated_lux <= range.middle) {
                    hyst_min_ = range.min;
                    hyst_max_ = range.max;
                    break;
                }
            }

            force_update_ = false;
            last_corrected_value_ = final_lux;
            last_calculated_lux_ = calculated_lux;
        } else {
            LOG(VERBOSE) << "Screenshot capture failed";
            final_lux = calculated_lux;
            last_corrected_value_ = calculated_lux;
            last_calculated_lux_ = calculated_lux;
        }
    } else if (calculated_lux >= hyst_min_ && calculated_lux <= hyst_max_) {
        // Within hysteresis range, use cached value for stability
        final_lux = last_corrected_value_;
        LOG(VERBOSE) << StringPrintf("Within hysteresis (%.1f-%.1f), using cached: %.2f lux",
                                     hyst_min_, hyst_max_, final_lux);
    } else {
        // Outside hysteresis and no screenshot, update cache
        last_corrected_value_ = calculated_lux;
        final_lux = calculated_lux;
        last_calculated_lux_ = calculated_lux;
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
