/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.ims;

import vendor.oplus.hardware.ims.IOplusImsRadioIndication;
import vendor.oplus.hardware.ims.IOplusImsRadioResponse;

@VintfStability
interface IOplusImsRadio {
    oneway void setCallback(IOplusImsRadioResponse responseCallback, IOplusImsRadioIndication indicationCallback);
    oneway void queryVopsStatus(int serial);
    oneway void sendOemCommand(int serial, int commandId, String data);
}
