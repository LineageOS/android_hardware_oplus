/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "libshims_thermal-engine.oplus"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

#include <dlfcn.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include <string>

#include "ThermalEngineConfig.h"

using android::base::ReadFileToString;
using android::base::Trim;
using android::base::WriteStringToFd;

namespace {

constexpr char kThermalEngineConfig[] = "/vendor/etc/thermal-engine.conf";
constexpr char kThermalControlConfig[] =
        "/odm/etc/temperature_profile/sys_thermal_control_config.xml";
constexpr char kHoraeTargetConfig[] = "/odm/etc/horae/horae_target.conf";
constexpr char kProjectName[] = "/proc/oplusVersion/prjName";

FILE* OpenGeneratedConfig(const char* mode) {
    std::string project;
    if (ReadFileToString(kProjectName, &project)) {
        project = Trim(project);
    }

    std::string config =
            GenerateThermalEngineConfig(kThermalControlConfig, kHoraeTargetConfig, project);
    if (config.empty()) {
        return nullptr;
    }

    int fd = memfd_create("thermal-engine.conf", MFD_CLOEXEC);
    if (fd < 0 || !WriteStringToFd(config, fd) || lseek(fd, 0, SEEK_SET) < 0) {
        PLOG(ERROR) << "Failed to create in-memory config";
        return nullptr;
    }

    return fdopen(fd, mode);
}

}  // namespace

extern "C" FILE* fopen(const char* path, const char* mode) {
    static auto fopen_orig = reinterpret_cast<decltype(fopen)*>(dlsym(RTLD_NEXT, "fopen"));

    if (path && strcmp(path, kThermalEngineConfig) == 0) {
        if (FILE* fp = OpenGeneratedConfig(mode)) {
            return fp;
        }
    }

    return fopen_orig(path, mode);
}
