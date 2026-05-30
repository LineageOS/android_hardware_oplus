/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <assert.h>
#include <fcntl.h>
#include <oplus/oplus_hbp.h>
#include <unistd.h>

int main() {
    auto fd = open("/dev/hbp0", O_RDWR);
    assert(fd != -1);

    usr_data data{};
    data.val = 1;

    auto ret = ioctl(fd, HBP_IOCTRL_IRQ_ENABLE, &data);
    assert(ret == 0);

    while (true) {
        sleep(1);
    }

    return 0;
}
