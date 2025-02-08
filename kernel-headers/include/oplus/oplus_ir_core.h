/*
 * Copyright (C) 2025 The LineageOS Project
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

#pragma once

#include <sys/ioctl.h>

#define IR_IOCTL_GROUP 0xE5

#define IR_GET_INF _IO(IR_IOCTL_GROUP, 0x01)
#define IR_SEND_PATTERN _IO(IR_IOCTL_GROUP, 0x02)

struct pattern_params {
    int32_t carrier_freq;
    uint32_t size;
    uint32_t pattern[];
};
