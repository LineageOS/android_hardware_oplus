/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

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
