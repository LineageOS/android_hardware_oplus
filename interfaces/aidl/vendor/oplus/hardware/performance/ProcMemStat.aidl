/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.performance;

@VintfStability
parcelable ProcMemStat {
    String comm;
    boolean is_32bit = false;
    int oom_score_adj = 0;
    int nr_fds = 0;
    int uid = 0;
    int pid = 0;
    int ppid = 0;
    int anon = 0;
    int file = 0;
    int shmem = 0;
    int swap = 0;
    int vss = 0;
    int pss = 0;
    int swap_rss = 0;
    int javaheap = 0;
    int nativeheap = 0;
    int ashmem = 0;
    int dmabuf = 0;
    int gpu = 0;
}
