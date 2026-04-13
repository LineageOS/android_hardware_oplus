/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys;

@VintfStability
@Backing(type="int")
enum SubsysError {
    NONE = 0,
    NOT_SUPPORT = 1,
    GENERIC_FAILURE = 2,
    EFS_FILE_NOT_FOUND = 3,
    OP_NOT_ALLOWED = 4,
    SERVICE_NOT_AVAILABLE = 5,
    SIM_ABSENT = 6,
    SIM_ERROR = 7,
    SIM_NO_SUCH_ELEMENT = 8,
    SIM_INVALID_STATE = 9,
    SUBSYS_E_SIM_PATH_ERROR = 10,
    TIMEOUT = 11,
    INTERNAL_ERROR = 12,
}
