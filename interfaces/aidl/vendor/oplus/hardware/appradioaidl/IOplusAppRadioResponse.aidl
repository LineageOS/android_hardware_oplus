/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.appradioaidl;

@VintfStability
interface IOplusAppRadioResponse {
    oneway void setNecReportPeriodResponse(int serial, int error);
    oneway void setNecConfigResponse(int serial, int error);
    oneway void getNecDataResponse(int serial, int error, in byte[] payload);
    oneway void getIcdDataResponse(int serial, int error, in String[] payload);
    oneway void getDmfNwDataResponse(int serial, int error, String payload);
}
