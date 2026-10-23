/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

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
