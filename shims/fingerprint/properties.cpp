/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "libshims_fingerprint.oplus"

#include <cutils/log.h>

#include <dlfcn.h>
#include <string.h>

extern "C" int property_get(const char *key, char *value, const char *default_value) {
    static auto property_get_orig = reinterpret_cast<typeof(property_get) *>(
            dlsym(RTLD_NEXT, "property_get"));

    if (strcmp(key, "ro.boot.vbmeta.device_state") == 0) {
        ALOGV("Returning unlocked for ro.boot.vbmeta.device_state");
        return strlen(strcpy(value, "unlocked"));
    }

    if (strcmp(key, "ro.boot.verifiedbootstate") == 0) {
        ALOGV("Returning orange for ro.boot.verifiedbootstate");
        return strlen(strcpy(value, "orange"));
    }

    return property_get_orig(key, value, default_value);
}
