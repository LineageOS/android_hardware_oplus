/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
///////////////////////////////////////////////////////////////////////////////
// THIS FILE IS IMMUTABLE. DO NOT EDIT IN ANY CASE.                          //
///////////////////////////////////////////////////////////////////////////////

// This file is a snapshot of an AIDL file. Do not edit it manually. There are
// two cases:
// 1). this is a frozen version file - do not edit this in any case.
// 2). this is a 'current' file. If you make a backwards compatible change to
//     the interface (from the latest frozen version), the build system will
//     prompt you to update this file with `m <name>-update-api`.
//
// You must not make a backward incompatible change to any AIDL file built
// with the aidl_interface module type with versions property set. The module
// type is used to build AIDL files in a way that they can be used across
// independently updatable components of the system. If a device is shipped
// with such a backward incompatible change, it has a high risk of breaking
// later when a module using the interface is updated, e.g., Mainline modules.

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
