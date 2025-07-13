/*
 * Copyright (C) 2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.Oem_key_log_err_msg_type;
import vendor.oplus.hardware.radio.Oem_log_packet_type;

@VintfStability
interface IOplusRadioIndication {
    void networkInfoInd(int type, in String[] networkInfo);
    void oemHotswapProcessInd(int type, int process, int state);
    void oemHotswapSetInd(int type, int state);
    void oemKeyLogErrInd(int type, in Oem_key_log_err_msg_type errMsg);
    void oemLargeDataKeyLogErrInd(int type, in int[] errData);
    void oemLogPacketInd(int type, in Oem_log_packet_type logPacket);
    void oemPinPukRetryNumInd(int type, int pin1, int pin2, int puk1);
    void smlDeviceLockInfoChangedInd(int type, String lockInfo);
    void smlEncryptedSerialIdInd(int type, in String[] serialIds);
    void subsidyDeviceLockInfoChangedInd(int type, String lockInfo);
    void regionlockStatusChangedInd(int type, in byte[] status);
    void nrEvolutionTypeIndication(int type, int evolutionType);
    void singleSimConfigChangedInd(int type, int config);
    void offlineLockStatusChangedInd(int type, in int[] status);
}
