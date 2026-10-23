/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
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

package vendor.oplus.hardware.subsys_interface.subsys_radio;
@VintfStability
parcelable ArrearageCfgInfo {
  int option = 0;
  boolean enable = false;
  int oos_duration_level_1 = 0;
  int oos_duration_level_2 = 0;
  int oos_duration_level_3 = 0;
  int oos_duration_level_4 = 0;
  int oos_duration_level_5 = 0;
  int short_limit_timer_1 = 0;
  int short_limit_timer_2 = 0;
  int short_limit_timer_3 = 0;
  int watch_timer = 0;
  int repeat_unit = 0;
  int long_limit_timer = 0;
  int enter_judge_screen = 0;
  int exit_judge_screen = 0;
  int enter_judge_inactive_mode = 0;
  int exit_judge_inactive_mode = 0;
  int ls_limit_timer_1 = 0;
  int ls_limit_timer_2 = 0;
  int ls_limit_timer_3 = 0;
  int ls_watch_timer = 0;
  int ls_repeat_unit = 0;
  int ls_duration_level = 0;
}
