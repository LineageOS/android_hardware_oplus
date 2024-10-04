/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Fingerprint.h"

#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/strings.h>
#include <android/binder_manager.h>

#include <fingerprint.sysprop.h>

using ::android::base::ParseInt;
using ::android::base::Split;

using namespace ::android::fingerprint::oplus;

namespace aidl::android::hardware::biometrics::fingerprint {
namespace {
constexpr int SENSOR_ID = 0;
constexpr common::SensorStrength SENSOR_STRENGTH = common::SensorStrength::STRONG;
constexpr int MAX_ENROLLMENTS_PER_USER = 4;
constexpr char HW_COMPONENT_ID[] = "fingerprintSensor";
constexpr char HW_VERSION[] = "vendor/model/revision";
constexpr char FW_VERSION[] = "1.0";
constexpr char SERIAL_NUMBER[] = "00000001";
constexpr char SW_COMPONENT_ID[] = "matchingAlgorithm";
constexpr char SW_VERSION[] = "vendor/version/revision";
}  // namespace

Fingerprint::Fingerprint() {
    const std::string instance = std::string() + Fingerprint::descriptor + "/oplus";
    mOplusFingerprint =
            IFingerprint::fromBinder(ndk::SpAIBinder(AServiceManager_getService(instance.c_str())));

    std::string sensorTypeProp = FingerprintHalProperties::type().value_or("");
    if (sensorTypeProp == "" || sensorTypeProp == "default" || sensorTypeProp == "rear")
        mSensorType = FingerprintSensorType::REAR;
    else if (sensorTypeProp == "udfps")
        mSensorType = FingerprintSensorType::UNDER_DISPLAY_ULTRASONIC;
    else if (sensorTypeProp == "udfps_optical")
        mSensorType = FingerprintSensorType::UNDER_DISPLAY_OPTICAL;
    else if (sensorTypeProp == "side")
        mSensorType = FingerprintSensorType::POWER_BUTTON;
    else if (sensorTypeProp == "home")
        mSensorType = FingerprintSensorType::HOME_BUTTON;
    else
        mSensorType = FingerprintSensorType::UNKNOWN;

    mMaxEnrollmentsPerUser =
            FingerprintHalProperties::max_enrollments_per_user().value_or(MAX_ENROLLMENTS_PER_USER);

    return;
}

ndk::ScopedAStatus Fingerprint::getSensorProps(std::vector<SensorProps>* out) {
    std::vector<common::ComponentInfo> componentInfo = {
            {HW_COMPONENT_ID, HW_VERSION, FW_VERSION, SERIAL_NUMBER, "" /* softwareVersion */},
            {SW_COMPONENT_ID, "" /* hardwareVersion */, "" /* firmwareVersion */,
             "" /* serialNumber */, SW_VERSION}};
    common::CommonProps commonProps = {SENSOR_ID, SENSOR_STRENGTH, mMaxEnrollmentsPerUser,
                                       componentInfo};

    SensorLocation sensorLocation;
    std::string loc = FingerprintHalProperties::sensor_location().value_or("");
    std::vector<std::string> dim = Split(loc, "|");
    if (dim.size() >= 3 && dim.size() <= 4) {
        ParseInt(dim[0], &sensorLocation.sensorLocationX);
        ParseInt(dim[1], &sensorLocation.sensorLocationY);
        ParseInt(dim[2], &sensorLocation.sensorRadius);
        if (dim.size() >= 4) {
            sensorLocation.display = dim[3];
        }
    } else if (loc.length() > 0) {
        LOG(WARNING) << "Invalid sensor location input (x|y|radius|display): " << loc;
    }

    LOG(INFO) << "Sensor type: " << ::android::internal::ToString(mSensorType)
              << " location: " << sensorLocation.toString();

    *out = {{commonProps, mSensorType, {sensorLocation}, false, false, false, false, std::nullopt}};

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Fingerprint::createSession(int32_t sensorId, int32_t userId,
                                              const std::shared_ptr<ISessionCallback>& cb,
                                              std::shared_ptr<ISession>* out) {
    return mOplusFingerprint->createSession(sensorId, userId, cb, out);
}

}  // namespace aidl::android::hardware::biometrics::fingerprint
