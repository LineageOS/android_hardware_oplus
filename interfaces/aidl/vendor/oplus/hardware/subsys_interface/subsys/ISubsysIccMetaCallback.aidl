/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys;

import vendor.oplus.hardware.subsys_interface.subsys.IccMetaRespInfo;

@VintfStability
interface ISubsysIccMetaCallback {
    void onIccMetaResp(in IccMetaRespInfo respInfo);
}
