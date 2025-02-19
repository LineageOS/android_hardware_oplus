/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "OPlusSensorPropsShim"

#include <aidl/android/hardware/biometrics/fingerprint/SensorProps.h>

#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

#include <dlfcn.h>

using android::base::GetProperty;
using android::base::ParseInt;
using android::base::Tokenize;

using aidl::android::hardware::biometrics::fingerprint::FingerprintSensorType;
using aidl::android::hardware::biometrics::fingerprint::SensorProps;

namespace {
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

    auto loc_prop = GetProperty("persist.vendor.fingerprint.optical.sensorlocation", "");
    if (!loc_prop.empty()) {
        auto loc = Tokenize(loc_prop, ":");
        bool loc_parsed = false;
        if (loc.size() >= 2) {
            int32_t x, y;
            loc_parsed = ParseInt(loc[0], &x) && ParseInt(loc[1], &y);
            if (loc_parsed) {
                props.sensorLocations[0].sensorLocationX = x;
                props.sensorLocations[0].sensorLocationY = y;
            }
        }
        LOG_IF(WARNING, !loc_parsed) << "Invalid sensor location input (x::y): " << loc_prop;
    }

    auto size = GetProperty("persist.vendor.fingerprint.optical.iconsize", "");
    if (!size.empty()) {
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
