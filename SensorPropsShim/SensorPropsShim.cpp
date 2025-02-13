/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <aidl/android/hardware/biometrics/fingerprint/SensorProps.h>
#include <dlfcn.h>

using aidl::android::hardware::biometrics::fingerprint::SensorProps;

SensorProps sensorProps;

extern "C" void
_ZNK4aidl7android8hardware10biometrics11fingerprint11SensorProps13writeToParcelEP7AParcel(
        SensorProps* thisptr, AParcel* parcel) {
    static auto writeToParcel = reinterpret_cast<
            typeof(_ZNK4aidl7android8hardware10biometrics11fingerprint11SensorProps13writeToParcelEP7AParcel)*>(
            dlsym(RTLD_NEXT, __func__));

    writeToParcel(&sensorProps, parcel);
}

__attribute__((constructor)) void SensorPropsInit() {
    // customize everything here
    sensorProps.supportsDetectInteraction = true;
}
