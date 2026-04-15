/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_imsrtp;

import vendor.oplus.hardware.subsys_imsrtp.CallEndInfo;
import vendor.oplus.hardware.subsys_imsrtp.RedunSystemInfo;

@VintfStability
interface IImsRtpIndication {
    void redunSystemChanged(int type, in RedunSystemInfo info);
    void notifyCallEndInfo(int type, in CallEndInfo info);
}
