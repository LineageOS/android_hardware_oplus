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

package vendor.oplus.hardware.communicationcenter;
@Backing(type="int") @VintfStability
enum DmtpErrorCode {
  UNKNOWN_ERROR = (-5) /* -5 */,
  ERR_INVAILD_PARAM = (-4) /* -4 */,
  ERR_INVAILD_OPERATE = (-3) /* -3 */,
  ERR_UNSUPPORTED = (-2) /* -2 */,
  ERR_SERVICE_UNAVAILABLE = (-1) /* -1 */,
  NO_ERR = 0,
  ERR_URL = 100,
  ERR_SERVICE_DISCONNECTED = 200,
  ERR_TRANSFER_TIMEOUT = 400,
}
