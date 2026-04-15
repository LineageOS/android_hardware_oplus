/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_imsrtp;

import vendor.oplus.hardware.subsys_imsrtp.IImsRtpIndication;
import vendor.oplus.hardware.subsys_imsrtp.IImsRtpResponse;
import vendor.oplus.hardware.subsys_imsrtp.ImsRtpCallInfo;
import vendor.oplus.hardware.subsys_imsrtp.MediaCapabilityRedunInfo;
import vendor.oplus.hardware.subsys_imsrtp.RedunControlInfo;

@VintfStability
interface IImsRtp {
    void setCallback(IImsRtpResponse iImsRtpResponse, IImsRtpIndication iImsRtpIndication);
    void setCallInfo(int serial, in ImsRtpCallInfo imsRtpCallInfo);
    void setRedunControlInfo(int serial, in RedunControlInfo redunControlInfo);
    void setMediaCapabilityRedunInfo(int serial, in MediaCapabilityRedunInfo mediaCapabilityRedunInfo);
}
