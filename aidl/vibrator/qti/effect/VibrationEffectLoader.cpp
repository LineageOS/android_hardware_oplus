/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "VibrationEffectLoader"

#include "VibrationEffectLoader.h"

#include <android-base/logging.h>

#include <fstream>

/*
 * Example of valid vibrator_effect.json:
 *
 * {
 *   "9999": {
 *     "def_style": [
 *       {
 *         "effect_file": "/odm/etc/vibrator/9999/def/effect_0.bin",
 *         "effect_id": 0,
 *         "play_rate_hz": 24000
 *       },
 *       ...
 *     ],
 *     "soft_style": [
 *       {
 *         "effect_file": "/odm/etc/vibrator/9999/soft/effect_0.bin",
 *         "effect_id": 0,
 *         "play_rate_hz": 24000
 *       },
 *       ...
 *     ]
 *   },
 *   ...
 * }
 */

namespace {
const auto kConfigPath = "/odm/etc/vibrator/vibrator_effect.json";

const auto kKeyDefStyle = "def_style";
const auto kKeyEffectFile = "effect_file";
const auto kKeyEffectId = "effect_id";
const auto kKeyPlayRateHz = "play_rate_hz";

const int8_t* LoadEffectDataFromFile(std::string_view path, uint32_t& length) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        LOG(ERROR) << "Failed to open data file: " << path;
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    length = file.tellg();
    file.seekg(0, std::ios::beg);

    int8_t* data = new int8_t[length];
    if (!file.read(reinterpret_cast<char*>(data), length)) {
        LOG(ERROR) << "Failed to read data file: " << path;
        delete[] data;
        return nullptr;
    }

    LOG(DEBUG) << "Successfully read data from " << path << ", length=" << length;

    return data;
}
};  // anonymous namespace

VibrationEffectLoader::VibrationEffectLoader() {
    std::ifstream config_stream(kConfigPath);
    if (!config_stream) {
        LOG(INFO) << "Couldn't open " << kConfigPath
                  << " for parsing, falling back to built-in effects.";
        return;
    }
    auto node = parseEffectJson(config_stream);
    if (node.isNull()) {
        LOG(ERROR) << "Failed to parse " << kConfigPath << ", falling back to built-in effects.";
        return;
    }
    loadEffects(std::move(node));
}

VibrationEffectLoader::~VibrationEffectLoader() {
    std::for_each(effect_map_.begin(), effect_map_.end(), [](auto&& v) { delete[] v.second.data; });
}

effect_stream* VibrationEffectLoader::getEffectStream(uint32_t effect_id) {
    auto entry = effect_map_.find(effect_id);
    if (entry != effect_map_.end()) {
        return &entry->second;
    }
    return nullptr;
}

Json::Value VibrationEffectLoader::parseEffectJson(std::ifstream& config_stream) {
    Json::Value root;
    Json::CharReaderBuilder builder;
    std::string errs;

    if (!Json::parseFromStream(builder, config_stream, &root, &errs)) {
        LOG(ERROR) << "Failed to parse config JSON stream, error: " << errs;
        return Json::Value::null;
    }
    if (!root.isObject()) {
        LOG(ERROR) << "Root must be an object";
        return Json::Value::null;
    }

    for (auto&& node : root) {
        if (node.isObject()) {
            // TODO: also load soft_style when dynamic switching is supported
            node = node[kKeyDefStyle];
            if (node.isArray() && !node.empty()) {
                return node;
            }
        }
    }
    LOG(ERROR) << "No effect node found in config";
    return Json::Value::null;
}

void VibrationEffectLoader::loadEffects(Json::Value&& effect_nodes) {
    for (auto&& node : effect_nodes) {
        auto attr = node[kKeyEffectId];
        if (!attr.isUInt()) {
            LOG(ERROR) << "Invalid effect_id";
            continue;
        }
        uint32_t effect_id = attr.asUInt();

        attr = node[kKeyPlayRateHz];
        if (!attr.isUInt()) {
            LOG(ERROR) << "Invalid play_rate_hz";
            continue;
        }
        uint32_t play_rate_hz = attr.asUInt();

        uint32_t length;
        auto data = LoadEffectDataFromFile(node[kKeyEffectFile].asString(), length);
        if (!data) {
            continue;
        }

        effect_map_.emplace(effect_id, effect_stream{effect_id, length, play_rate_hz, data});
    }
}
