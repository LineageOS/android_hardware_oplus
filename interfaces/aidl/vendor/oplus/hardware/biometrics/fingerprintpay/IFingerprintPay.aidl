/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.biometrics.fingerprintpay;

@VintfStability
interface IFingerprintPay {
    int cancel(String str);
    int enable_rpmb();
    int get_ifaa_key_status();
    int get_rpmb_enable_state();
    int get_rpmb_state();
    byte[] ifaa_invoke_command(in byte[] arr);
    int ifaa_key_verify();
    int ifaa_key_write();
}
