/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.appradioaidl;

import vendor.oplus.hardware.appradioaidl.IOplusAppRadioIndication;
import vendor.oplus.hardware.appradioaidl.IOplusAppRadioResponse;

@VintfStability
interface IOplusAppRadio {
    oneway void setCallback(IOplusAppRadioResponse response, IOplusAppRadioIndication indication);
    oneway void setNecReportPeriod(int serial, long period);
    oneway void setNecConfigRequest(int serial, int command, in byte[] data, byte value);
    oneway void getNecDataRequest(int serial, int command, in byte[] data);
    oneway void getIcdDataRequest(int serial, int command, String payload);
    oneway void getDmfNwDataRequest(int serial, int command, int mode);
}
