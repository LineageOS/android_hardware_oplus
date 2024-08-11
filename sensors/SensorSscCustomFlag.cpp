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
#include <sys/mman.h>
#include <sys/user.h>

extern "C" void* sensorsHalGetSubHal(uint32_t* version) {
    static auto handle = dlopen("sensors.ssc", RTLD_NOW);
    static auto sensorsHalGetSubHal_orig =
            reinterpret_cast<typeof(sensorsHalGetSubHal)*>(dlsym(handle, "sensorsHalGetSubHal"));
    auto ret = sensorsHalGetSubHal_orig(version);

    // Get VTable
    auto vtable = reinterpret_cast<void**>(ret);
    auto vtable_aligned = reinterpret_cast<void**>(uintptr_t(vtable) & ~(PAGE_SIZE - 1));

    // Modify VTable
    mprotect(vtable_aligned, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
    vtable[23] = vtable[24];  // setCustomFlag -> getName
    vtable[24] = vtable[25];  // getName -> initialize
    vtable[25] = nullptr;     // initialize -> nullptr
    mprotect(vtable_aligned, PAGE_SIZE, PROT_READ | PROT_EXEC);

    return ret;
}
