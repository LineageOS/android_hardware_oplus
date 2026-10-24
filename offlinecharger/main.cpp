/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>
#include <android-base/unique_fd.h>

#include <fcntl.h>
#include <unistd.h>

#include <cstring>

using android::base::ReadFullyAtOffset;
using android::base::StartsWith;
using android::base::unique_fd;
using android::base::WriteFullyAtOffset;

namespace {

// ABL boots into offline charging on charger insertion only if this flag is set,
// and clears it on every boot.
constexpr auto kChargeInfoPartition = "/dev/block/by-name/oplusreserve1";
constexpr auto kChargeInfoOffset = 0x48a000;
constexpr char kChargeInfoMagic[] = "oplus_charge";
constexpr auto kKpocFlagOffset = kChargeInfoOffset + 0xe;
constexpr char kKpocFlag[] = "allow_kpoc";

}  // namespace

int main(int argc, char** argv) {
    // sys.shutdown.requested is "0<reason>" for power off and "1<reason>" for reboot.
    if (argc != 2 || !StartsWith(argv[1], "0")) {
        return 0;
    }

    unique_fd fd(open(kChargeInfoPartition, O_RDWR | O_CLOEXEC));
    CHECK(fd.ok());

    char magic[sizeof(kChargeInfoMagic)];
    CHECK(ReadFullyAtOffset(fd, magic, sizeof(magic), kChargeInfoOffset));
    CHECK(memcmp(magic, kChargeInfoMagic, sizeof(magic)) == 0);

    CHECK(WriteFullyAtOffset(fd, kKpocFlag, sizeof(kKpocFlag), kKpocFlagOffset));
    CHECK(fsync(fd) == 0);

    return 0;
}
