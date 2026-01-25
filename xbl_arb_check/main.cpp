//
// SPDX-FileCopyrightText: 2026 The LineageOS Project
// SPDX-License-Identifier: Apache-2.0
//

#include <android-base/properties.h>
#include <elf.h>
#include <selinux/selinux.h>
#include <fstream>
#include <optional>
#include <vector>

using android::base::GetProperty;

struct Elf64_ShdrOem : Elf64_Shdr {
    // https://github.com/NichtsHsu/mbn-rs/blob/da68f5c38b87df3a63bc6b50b613ad2cbb0b4451/mbn/src/metadata.rs#L28
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

        auto segment = reinterpret_cast<Elf64_ShdrOem*>(&buf[program_header->p_offset]);

        if (segment->sh_type != SHT_NOTE || segment->oem_id != 0x51) {
            continue;
        }

        return segment->anti_rollback_version;
    }

    return {};
}

int main() {
    if (!std::ifstream("/dev/block/by-name/xbl_config_a").good() ||
        !std::ifstream("/dev/block/by-name/xbl_config_b").good()) {
        printf("xbl_arb_check: xbl_config_a or xbl_config_b cannot be read, assuming everything is "
               "ok\n");
        return EXIT_SUCCESS;
    }

    auto arb_current = read_anti_rollback_version("/dev/block/by-name/xbl_config_a");

    if (!arb_current) {
        printf("xbl_arb_check: Failed to read xbl_config_a ARB value\n");
        return EXIT_FAILURE;
    }

    auto arb_other = read_anti_rollback_version("/dev/block/by-name/xbl_config_b");

    if (!arb_other) {
        printf("xbl_arb_check: Failed to read xbl_config_b ARB value\n");
        return EXIT_FAILURE;
    }

    if (GetProperty("ro.boot.slot_suffix", "") == "_b") {
        std::swap(arb_current, arb_other);
    }

    printf("xbl_arb_check: arb_current: %u, arb_other: %u\n", *arb_current, *arb_other);

    return arb_current > arb_other;
}
