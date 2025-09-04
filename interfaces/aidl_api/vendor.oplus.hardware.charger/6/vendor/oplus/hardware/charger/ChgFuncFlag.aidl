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

package vendor.oplus.hardware.charger;
@Backing(type="int") @VintfStability
enum ChgFuncFlag {
  GET_UI_CHG_ICON_TYPE = 0,
  GET_UI_POWER_VALUE = 1,
  GET_DEVICE_POWER_VALUE = 2,
  GET_ADAPTER_POWER_VALUE = 3,
  GET_CPA_POWER_VALUE = 4,
  SET_CHG_UP_LIMIT_VALUE = 5,
  IS_SUPPORT_SINGLE_HYPER_SPEED = 6,
  GET_BATT_EXPANSION_STATE = 7,
  IS_SUPPORT_ECO_DESIGN = 8,
  GET_BATT_MANU_DATE = 9,
  GET_BATT_FIRST_USAGE_DATE = 10,
  SET_BATT_FIRST_USAGE_DATE = 11,
  CLEAR_BATT_ECO_DATA = 12,
  SET_RECHG_SOC_EN = 13,
  SET_CHG_UP_LIMIT_STATE = 14,
  GET_BATTERY_STATUS_PM_VPH = 15,
  GET_BATTERY_STATUS_PM_VBAT = 16,
  GET_DEEP_DISCHG_COUNTS_VALUE = 17,
  GET_ANTI_EXPANSION_INFO = 18,
  SET_ANTI_EXPANSION_INFO = 19,
  GET_BATT_UI_CYCLE_COUNT = 20,
  GET_BATTERY_TYPE = 21,
  WIRELESS_PENCIL_QCALI = 22,
  GET_BATTERY_GAUGE_TYPE = 23,
  GET_CHG_BYPASS_STATUS = 24,
  SET_CHG_BYPASS = 25,
}
