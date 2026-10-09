/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/logging.h>

#include <stdio.h>

#include <string>

#include "ThermalEngineConfig.h"

int main(int argc, char** argv) {
    android::base::InitLogging(argv, android::base::StderrLogger);

    if (argc < 3 || argc > 4) {
        fprintf(stderr,
                "usage: %s <sys_thermal_control_config.xml> <horae_target.conf> [project]\n",
                argv[0]);
        return 1;
    }

    std::string config = GenerateThermalEngineConfig(argv[1], argv[2], argc > 3 ? argv[3] : "");
    if (config.empty()) {
        return 1;
    }

    fputs(config.c_str(), stdout);
    return 0;
}
