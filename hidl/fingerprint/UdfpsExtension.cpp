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

#include <compositionengine/UdfpsExtension.h>

#if __has_include(<display/drm/sde_drm.h>)
#include <display/drm/sde_drm.h>
#elif __has_include(<drm/sde_drm.h>)
#include <drm/sde_drm.h>
#endif

uint32_t getUdfpsAodZOrder(uint32_t z) {
    return 0x41000011;
}

uint32_t getUdfpsDimZOrder(uint32_t z) {
    return 0x41000005;
}

uint32_t getUdfpsZOrder(uint32_t z, bool touched) {
#ifdef FOD_PRESSED_LAYER_ZORDER
    return touched ? z | FOD_PRESSED_LAYER_ZORDER : z;
#else
    return touched ? 0x41000033 : 0x41000032;
#endif
}

uint64_t getUdfpsUsageBits(uint64_t usageBits, bool /*touched*/) {
    return usageBits;
}
