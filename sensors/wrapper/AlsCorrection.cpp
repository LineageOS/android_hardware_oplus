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

void ParseBrightnessRanges(const Json::Value& arr, std::vector<BrightnessRange>& out) {
    out.resize(arr.size());
    for (const Json::Value& item : arr) {
        int level = item["Level"].asInt();
        out[level] = {item["BrightnessMin"].asInt(), item["BrightnessMax"].asInt()};
    }
}

void ParseLuxCoeff(const Json::Value& arr, std::vector<LuxCoeff>& out) {
    out.resize(arr.size());
    for (const Json::Value& item : arr) {
        int level = item["Level"].asInt();
        out[level] = {ParseFloat(item["ChannelR"]), ParseFloat(item["ChannelG"]),
                      ParseFloat(item["ChannelB"]), ParseFloat(item["ChannelC"])};
    }
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
                    // TODO: properly handle SPECIAL_PICTURE if needed
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
    ParseBrightnessRanges(
            doc.isMember("IRBrightness") ? doc["IRBrightness"] : doc["IRBrightness_V2_1"],
            conf_.ir_brightness);

    // Parse LuxCoeffLIR
    ParseLuxCoeff(doc.isMember("LuxCoeffLIR") ? doc["LuxCoeffLIR"] : doc["LuxCoeffLIR_V2_1"],
                  conf_.lux_coeff_lir);

    // Parse LuxCoeffHIR
    ParseLuxCoeff(doc.isMember("LuxCoeffHIR") ? doc["LuxCoeffHIR"] : doc["LuxCoeffHIR_V2_1"],
                  conf_.lux_coeff_hir);

    // Parse LuxCoeffSuperHIR
    ParseLuxCoeff(doc.isMember("LuxCoeffSuperHIR") ? doc["LuxCoeffSuperHIR"]
                                                   : doc["LuxCoeffSuperHIR_V2_1"],
                  conf_.lux_coeff_super_hir);

    // Parse LinearityBrightnessRange
    ParseBrightnessRanges(doc["LinearityBrightnessRange"], conf_.linearity_ranges);

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

        // sanity check
        if (conf_.linearity_ranges.size() != conf_.linearity.size()) {
            LOG(ERROR) << "Linearity size and range mismatch";
            return false;
        }
    }

    // Parse Golden
    {
        if (doc.isMember("Golden")) {
            const Json::Value& arr = doc["Golden"];
            for (const Json::Value& item : arr) {
                int ch = item["Channel"].asInt();
                conf_.golden[ch] = {ParseInt(item["RGolden"]), ParseInt(item["GGolden"]),
                                    ParseInt(item["BGolden"]), ParseInt(item["WGolden"])};
            }
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
        } else {
            LOG(ERROR) << "No Golden or LightLeakageGolden found in config";
            return false;
        }
    }

    // Parse GreyScale (optional)
    {
        const Json::Value& arr = doc["GreyScale"];
        if (!arr.isNull()) {
            for (const Json::Value& item : arr) {
                int ch = item["Channel"].asInt();
                conf_.grayscale[ch] = {ParseFloat(item["RGreyscale"]),
                                       ParseFloat(item["GGreyscale"]),
                                       ParseFloat(item["BGreyscale"])};
            }
        }
    }

    // Check for L_ mode (optional)
    {
        const Json::Value& bri_arr = doc["L_IRBrightness"];
        const Json::Value& lux_arr = doc["L_LuxCoeffLIR"];
        if (!bri_arr.isNull() && bri_arr.size() == lux_arr.size()) {
            conf_.has_l_mode = true;
            ParseBrightnessRanges(bri_arr, conf_.l_ir_brightness);
            ParseLuxCoeff(lux_arr, conf_.l_lux_coeff_lir);
        }
    }

    // Check for M_ mode (optional)
    {
        const Json::Value& bri_arr = doc["M_IRBrightness"];
        const Json::Value& lux_arr = doc["M_LuxCoeffLIR"];
        if (!bri_arr.isNull() && bri_arr.size() == lux_arr.size()) {
            conf_.has_m_mode = true;
            ParseBrightnessRanges(bri_arr, conf_.m_ir_brightness);
            ParseLuxCoeff(lux_arr, conf_.m_lux_coeff_lir);
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

float AlsCorrection::applyLinearityCorrection(float x, int linearity_level, int channel) {
    // Apply cubic polynomial: p3*x³ + p2*x² + p1*x + p0
    const LinearityParams& params = conf_.linearity[linearity_level].channels[channel];
    return params.p3 * x * x * x + params.p2 * x * x + params.p1 * x + params.p0;
}

int AlsCorrection::determineIRLevel(float brightness, float ir_ratio,
                                    const BrightnessRange* ir_bright) {
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

    // Find appropriate linearity function based on brightness
    auto it_range = std::find_if(
            conf_.linearity_ranges.begin(), conf_.linearity_ranges.end(), [&](const auto& r) {
                return brightness >= r.bright_min && brightness <= r.bright_max;
            });
    if (it_range == conf_.linearity_ranges.end()) {
        return raw_clear;
    };

    int linearity_level = std::distance(conf_.linearity_ranges.begin(), it_range);

    // Apply linearity correction to clear channel (channel 3 = C/White)
    float corrected_clear = applyLinearityCorrection(raw_clear, linearity_level, 3);

    LOG(VERBOSE) << "Linearity corrected clear: " << corrected_clear;

    // For IR ratio calculation, we need RGB values
    float ir_ratio = 0.5;  // Default if we can't calculate

    // Determine display mode based on brightness
    auto [ir_bright, lux_lir] = [&]() {
        if (conf_.has_l_mode && brightness <= 1105) {
            LOG(VERBOSE) << "Using L_ mode (low brightness)";
            return std::make_pair(std::ref(conf_.l_ir_brightness), std::ref(conf_.l_lux_coeff_lir));
        } else if (conf_.has_m_mode && brightness > 1105 && brightness <= 1246) {
            LOG(VERBOSE) << "Using M_ mode (medium brightness)";
            return std::make_pair(std::ref(conf_.m_ir_brightness), std::ref(conf_.m_lux_coeff_lir));
        } else {
            return std::make_pair(std::ref(conf_.ir_brightness), std::ref(conf_.lux_coeff_lir));
        }
    }();
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
                for (const auto& seg : conf_.cct_segments) {
                    // For very low ambient (pitch black room), bias toward segment 0 threshold
                    // detection Use segment if calculated_lux is within 10 lux of min (helps
                    // catch edge cases)
                    int min_threshold = std::max(0, seg.lux_min - 10);
                    if (calculated_lux >= min_threshold && calculated_lux <= seg.lux_max) {
                        active_segment = &seg;
                        LOG(VERBOSE)
                                << StringPrintf("Selected CCT segment for lux %.2f (range %d-%d)",
                                                calculated_lux, seg.lux_min, seg.lux_max);
                        break;
                    }
                }

                // Estimate screen brightness contribution
                float brightness_ratio =
                        static_cast<float>(brightness) / conf_.common.brightness_max;
                float golden_scale = (conf_.light_leakage_golden.empty()
                                              ? conf_.golden[3].w
                                              : conf_.light_leakage_golden[linearity_level].w) /
                                     1000.0f;
                float base_contrib = luminance * brightness_ratio * golden_scale;

                // Additional global boost for white pages in very dark rooms
                // This multiplier is applied on top of CCT segment boost
                constexpr float kDarkRoomWhiteBoost = 1.35f;  // +35% extra for pitch-black + white

                // Apply segment-specific leakage boost if color detected
                float leak_boost = 1.0f;
                if (active_segment != nullptr) {
                    // Check if this qualifies as GREY_SCREEN or WHITE_BLACK_SCREEN
                    for (const auto& pc : active_segment->pure_colors) {
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
                                        "CCT leak boost: color=%s, rgb_delta=%.1f, "
                                        "leak_proxy=%.3f, boost=%.2f",
                                        pc.color_name.c_str(), rgb_delta, leak_proxy, leak_boost);
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
