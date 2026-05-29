/*
 * SPDX-FileCopyrightText: 2025-2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "OPlusSensorPropsShim"

#include <aidl/android/hardware/biometrics/fingerprint/SensorProps.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

#include <dlfcn.h>

using android::base::GetIntProperty;
using android::base::GetProperty;
using android::base::ParseInt;
using android::base::ReadFileToString;
using android::base::Split;
using android::base::Tokenize;
using android::base::Trim;

using aidl::android::hardware::biometrics::fingerprint::FingerprintSensorType;
using aidl::android::hardware::biometrics::fingerprint::SensorLocation;
using aidl::android::hardware::biometrics::fingerprint::SensorProps;

namespace {
std::optional<std::pair<int32_t, int32_t>> GetScreenResolution() {
    std::string value;
    if (!ReadFileToString("/sys/class/drm/card0-DSI-1/modes", &value)) {
        return {};
    }

    auto lines = Split(Trim(value), "\n");
    if (lines.empty()) {
        return {};
    }

    auto parts = Split(lines.back(), "x");
    if (parts.size() < 2) {
        return {};
    }

    int32_t width, height;
    if (!ParseInt(parts[0], &width) || !ParseInt(parts[1], &height)) {
        return {};
    }

    return std::make_pair(width, height);
}

SensorProps SensorPropsInit(SensorProps props) {
    auto type = GetProperty("persist.vendor.fingerprint.sensor_type", "");
    if (!type.empty()) {
        if (type == "back")
            props.sensorType = FingerprintSensorType::REAR;
        else if (type == "ultrasonic")
            props.sensorType = FingerprintSensorType::UNDER_DISPLAY_ULTRASONIC;
        else if (type == "optical")
            props.sensorType = FingerprintSensorType::UNDER_DISPLAY_OPTICAL;
        else if (type == "side")
            props.sensorType = FingerprintSensorType::POWER_BUTTON;
        else if (type == "front")
            props.sensorType = FingerprintSensorType::HOME_BUTTON;
    }

    auto icon_loc_prop = GetIntProperty("persist.vendor.fingerprint.optical.iconlocation", 0);
    auto loc_prop = GetProperty("persist.vendor.fingerprint.optical.sensorlocation",
                                GetProperty("persist.vendor.fingerprint.side.sensorlocation", ""));
    auto screen_resolution = GetScreenResolution();
    if (icon_loc_prop > 0 && screen_resolution &&
        (props.sensorType == FingerprintSensorType::UNDER_DISPLAY_ULTRASONIC ||
         props.sensorType == FingerprintSensorType::UNDER_DISPLAY_OPTICAL)) {
        SensorLocation loc;
        loc.sensorLocationX = screen_resolution->first / 2;
        loc.sensorLocationY = screen_resolution->second - icon_loc_prop;
        props.sensorLocations = {loc};
    } else if (!loc_prop.empty()) {
        auto locations = Tokenize(loc_prop, "|");
        props.sensorLocations.clear();

        for (const auto& location : locations) {
            auto parts = Tokenize(location, "::");
            bool loc_parsed = false;
            if (parts.size() >= 2) {
                int32_t x, y;
                loc_parsed = ParseInt(parts[0], &x) && ParseInt(parts[1], &y);
                if (loc_parsed) {
                    SensorLocation loc;
                    loc.sensorLocationX = x;
                    loc.sensorLocationY = y;
                    if (parts.size() >= 3 && !parts[2].empty()) {
                        loc.display = "local:" + parts[2];
                    }

                    props.sensorLocations.push_back(loc);
                }
            }
            LOG_IF(WARNING, !loc_parsed)
                    << "Invalid sensor location input (x::y[::display]): " << location;
        }
    }

    auto size = GetProperty("persist.vendor.fingerprint.optical.iconsize", "");
    if (!size.empty() && !props.sensorLocations.empty()) {
        if (ParseInt(size, &props.sensorLocations[0].sensorRadius)) {
            props.sensorLocations[0].sensorRadius /= 2;
        } else {
            LOG(WARNING) << "Invalid sensor size input: " << size;
        }
    }

    props.halHandlesDisplayTouches =
            props.sensorType == FingerprintSensorType::UNDER_DISPLAY_OPTICAL ||
            props.sensorType == FingerprintSensorType::UNDER_DISPLAY_ULTRASONIC;

    return props;
}
}  // anonymous namespace

extern "C" void
_ZNK4aidl7android8hardware10biometrics11fingerprint11SensorProps13writeToParcelEP7AParcel(
        SensorProps* thisptr, AParcel* parcel) {
    static auto props = SensorPropsInit(*thisptr);
    static auto writeToParcel = reinterpret_cast<
            typeof(_ZNK4aidl7android8hardware10biometrics11fingerprint11SensorProps13writeToParcelEP7AParcel)*>(
            dlsym(RTLD_NEXT, __func__));

    LOG(DEBUG) << "Original props=" << thisptr->toString() << ", new props=" << props.toString();
    writeToParcel(&props, parcel);
}
