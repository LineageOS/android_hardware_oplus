/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_imsrtp;

import vendor.oplus.hardware.subsys_interface.subsys.SubsysResponseInfo;

@VintfStability
interface IImsRtpResponse {
    void setCallInfoResponse(in SubsysResponseInfo info);
    void setRedunControlInfoResponse(in SubsysResponseInfo info);
    void setMediaCapabilityRedunInfoResponse(in SubsysResponseInfo info);
}
