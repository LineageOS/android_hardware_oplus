/*
 * Copyright (C) 2026 The LineageOS Project
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

#include <stddef.h>
#include <stdint.h>
#include <sys/ioctl.h>

#define HBP_IOCTRL_GROUP 0xC5

#define HBP_IOCTRL_IRQ_ENABLE _IO(HBP_IOCTRL_GROUP, 0x0A)

#define MAX_POWER_SEQ (10)

enum power_type { POWER_AVDD = 0x1000, POWER_VDDI, POWER_RESET, POWER_BUS, POWER_MAX };

struct power_sequeue {
    enum power_type type;
    bool en;
    uint32_t msleep;
};

union usr_data {
    int64_t val;

    struct {
        void* tx;
        void* rx;
        int32_t len;
    } rw;

    struct {
        void* data;
        size_t size;
    } frame;

    struct {
        uint8_t state;
        int x;
        int y;
        int touch_early_down_flag;
        long is_touch_fp_area_Cnt;
        int tp_firmware_time;
    } ifp;

    struct power_sequeue sq[MAX_POWER_SEQ];

    struct {
        uint8_t mode;
        uint8_t bits_per_word;
        int speed;
    } spi_setup;

    struct {
        bool filmed;
        int level;
        bool trusty;
    } film;

    struct {
        void* info;
        size_t info_size;
    } health_info;
};
