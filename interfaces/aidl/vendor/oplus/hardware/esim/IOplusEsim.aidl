/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.esim;

@VintfStability
interface IOplusEsim {
    int getEsimGpio();
    void initEsimGpio();
    int setEsimGpio(int operation);
    int setHotswap();
    int setUimPower(int operation);
}
