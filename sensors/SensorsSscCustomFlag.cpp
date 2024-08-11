/*
 * Copyright (C) 2024 The LineageOS Project
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

#include <dlfcn.h>
#include <string.h>

extern "C" void* sensorsHalGetSubHal(uint32_t* version) {
    static auto sensorsHalGetSubHalOrig = reinterpret_cast<typeof(sensorsHalGetSubHal)*>(
            dlsym(dlopen("sensors.ssc.so", RTLD_NOW), "sensorsHalGetSubHal"));
    static void* ret = nullptr;

    if (ret == nullptr) {
        // Call original function
        ret = sensorsHalGetSubHalOrig(version);

        // Get original VTable
        auto vtableOrig = reinterpret_cast<void**>(ret);

        // Reallocate VTable
        static void* vtableNew[27];
        memcpy(vtableNew, &vtableOrig[-1], sizeof(vtableNew));
        *vtableOrig = &vtableNew[1];

        // Modify VTable
        vtableNew[24] = vtableNew[25];  // setCustomFlag -> getName
        vtableNew[25] = vtableNew[26];  // getName -> initialize
        vtableNew[26] = nullptr;        // initialize -> nullptr
    }

    return ret;
}
