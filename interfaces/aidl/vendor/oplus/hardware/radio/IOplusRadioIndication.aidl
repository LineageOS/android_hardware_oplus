/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.Oem_key_log_err_msg_type;
import vendor.oplus.hardware.radio.Oem_log_packet_type;

@VintfStability
interface IOplusRadioIndication {
    void networkInfoInd(int type, in String[] networkinfo);
    void oemHotswapProcessInd(int type, int slotid, int error);
    void oemHotswapSetInd(int type, int slotid);
    void oemKeyLogErrInd(int type, in Oem_key_log_err_msg_type errMsg);
    void oemLargeDataKeyLogErrInd(int type, in int[] result);
    void oemLogPacketInd(int type, in Oem_log_packet_type log_packet_msg);
    void oemPinPukRetryNumInd(int type, int slotid, int pin_retry_num, int puk_retry_num);
    void smlDeviceLockInfoChangedInd(int type, String info);
    void smlEncryptedSerialIdInd(int type, in String[] indStgs);
    void subsidyDeviceLockInfoChangedInd(int type, String info);
    void regionlockStatusChangedInd(int type, in byte[] region_lock_status_info);
    void nrEvolutionTypeIndication(int type, int toe);
    void singleSimConfigChangedInd(int type, int result);
    void offlineLockStatusChangedInd(int type, in int[] status);
}
