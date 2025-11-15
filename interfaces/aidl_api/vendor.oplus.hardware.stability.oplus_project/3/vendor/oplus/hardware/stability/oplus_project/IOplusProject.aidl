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

package vendor.oplus.hardware.stability.oplus_project;
@VintfStability
interface IOplusProject {
  int get_eng_version();
  String get_force_reboot();
  int get_ftmmode();
  String get_hungtask();
  String get_mt_gpio();
  String get_ocp();
  String get_opboot();
  int get_operator_name();
  String get_oplusocp_status(int pmic_id);
  String get_opluspoff_reason(int pmic_id);
  String get_opluspon_reason(int pmic_id);
  int get_pcb_version();
  String get_phoenix();
  String get_pmic_info();
  String get_poff_reason();
  String get_pon_reason();
  int get_project();
  int get_rf_type();
  int get_sau();
  String get_serial_ID();
  String get_shutdown_detect();
  boolean is_ufs_devices();
  String miscModule2Load();
  String miscModuleBlocklist();
  String miscReadLinkedModules();
  int[] readAgingData(int type);
  String read_theia_node(int theia_node_id);
  boolean saveAgingData(int type, in int[] saveAgingData, int length);
  boolean set_opboot(String opboot);
  boolean set_phoenix(String monitoring_command);
  boolean set_shutdown_detect(String shutdown_detect_value);
  boolean write_theia_node(int theia_node_id, String theia_data);
  String get_dram_freq();
  String read_op1_record_info();
  boolean write_op1_record_info(String op1_logs_data);
}
