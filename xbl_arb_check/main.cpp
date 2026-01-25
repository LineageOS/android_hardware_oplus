//
// SPDX-FileCopyrightText: 2026 The LineageOS Project
// SPDX-License-Identifier: Apache-2.0
//

#define LOG_TAG "xbl_arb_check"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <elf.h>
#include <fstream>
#include <optional>
#include <vector>

struct HeaderV6 {
    // https://github.com/NichtsHsu/mbn-rs/blob/da68f5c38b87df3a63bc6b50b613ad2cbb0b4451/mbn/src/header.rs#L109
    uint32_t image_id;
    uint32_t header_version;
    uint32_t qti_signature_size;
    uint32_t qti_cert_chain_size;
    uint32_t image_size;
    uint32_t code_size;
    uint32_t signature_ptr;
    uint32_t signature_size;
    uint32_t cert_chain_ptr;
    uint32_t cert_chain_size;
    uint32_t qti_metadata_size;
    uint32_t metadata_size;

    struct {
        // https://github.com/NichtsHsu/mbn-rs/blob/da68f5c38b87df3a63bc6b50b613ad2cbb0b4451/mbn/src/metadata.rs#L74
        uint32_t major_version;
        uint32_t minor_version;
        uint32_t software_id;
        uint32_t hardware_id;
        uint32_t oem_id;
        uint32_t model_id;
        uint32_t app_id;
        uint32_t flags;
        uint32_t soc_version[12];
        uint32_t multi_serial_numbers[8];
        uint32_t root_cert_index;
        uint32_t anti_rollback_version;
    } metadata;
};

struct HeaderV7 {
    // https://github.com/NichtsHsu/mbn-rs/blob/da68f5c38b87df3a63bc6b50b613ad2cbb0b4451/mbn/src/header.rs#L139
    uint32_t image_id;
    uint32_t header_version;
    uint32_t common_meta_size;
    uint32_t qti_metadata_size;
    uint32_t metadata_size;
    uint32_t code_size;
    uint32_t qti_signature_size;
    uint32_t qti_cert_chain_size;
    uint32_t signature_size;
    uint32_t cert_chain_size;

    struct {
        // https://github.com/NichtsHsu/mbn-rs/blob/da68f5c38b87df3a63bc6b50b613ad2cbb0b4451/mbn/src/metadata.rs#L42
        uint32_t major_version;
        uint32_t minor_version;
        uint32_t software_id;
        uint32_t app_id;
        uint32_t hash_table_algorithm;
        uint32_t measurement_register;
    } common_metadata;

    struct {
        // https://github.com/NichtsHsu/mbn-rs/blob/da68f5c38b87df3a63bc6b50b613ad2cbb0b4451/mbn/src/metadata.rs#L107
        uint32_t major_version;
        uint32_t minor_version;
        uint32_t anti_rollback_version;
        uint32_t root_cert_index;
        uint32_t soc_version[12];
        uint32_t feature_id;
        uint32_t hardware_id;
        uint64_t multi_serial_numbers[8];
        uint32_t oem_id;
        uint32_t model_id;
        uint64_t oem_lifecycle_state;
        uint32_t oem_root_cert_hash_algorithm;
        uint8_t oem_root_cert_hash[64];
        uint32_t flags;
    } metadata;
};

std::optional<uint32_t> read_anti_rollback_version(const std::string& path) {
    std::ifstream stream(path, std::ios::binary | std::ios::ate);

    if (!stream.good()) {
        return {};
    }

    std::vector<char> buf;
    buf.resize(stream.tellg());

    stream.seekg(0);
    stream.read(buf.data(), buf.size());

    auto elf_header = reinterpret_cast<Elf64_Ehdr*>(buf.data());

    for (int i = 0; i < elf_header->e_phnum; i++) {
        auto program_header = reinterpret_cast<Elf64_Phdr*>(
                &buf[elf_header->e_phoff + (i * elf_header->e_phentsize)]);

        if (program_header->p_type != PT_NULL) {
            continue;
        }

        auto header_v6 = reinterpret_cast<HeaderV6*>(&buf[program_header->p_offset]);

        if (header_v6->header_version == 6 &&
            header_v6->metadata_size == sizeof(header_v6->metadata)) {
            return header_v6->metadata.anti_rollback_version;
        }

        auto header_v7 = reinterpret_cast<HeaderV7*>(&buf[program_header->p_offset]);

        if (header_v7->header_version == 7 &&
            header_v7->metadata_size == sizeof(header_v7->metadata)) {
            return header_v7->metadata.anti_rollback_version;
        }
    }

    return {};
}

int main() {
    android::base::SetLogger(android::base::StdioLogger);

    if (!std::ifstream("/dev/block/by-name/xbl_config_a").good() ||
        !std::ifstream("/dev/block/by-name/xbl_config_b").good()) {
        LOG(ERROR) << "xbl_config_a or xbl_config_b cannot be read, assuming everything is ok";
        return EXIT_SUCCESS;
    }

    auto arb_current = read_anti_rollback_version("/dev/block/by-name/xbl_config_a");

    if (!arb_current) {
        LOG(ERROR) << "Failed to read xbl_config_a ARB value";
        return EXIT_FAILURE;
    }

    auto arb_other = read_anti_rollback_version("/dev/block/by-name/xbl_config_b");

    if (!arb_other) {
        LOG(ERROR) << "Failed to read xbl_config_b ARB value";
        return EXIT_FAILURE;
    }

    if (android::base::GetProperty("ro.boot.slot_suffix", "") == "_b") {
        std::swap(arb_current, arb_other);
    }

    LOG(INFO) << "arb_current: " << *arb_current << ", arb_other: " << *arb_other;

    return arb_current > arb_other;
}
