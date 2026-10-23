/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.radio;

import vendor.oplus.hardware.radio.Oem_key_log_err_msg_type;
import vendor.oplus.hardware.radio.Oem_log_packet_type;

@VintfStability
interface IOplusRadioIndication {
    oneway void networkInfoInd(int type, in String[] networkinfo);
    oneway void oemHotswapProcessInd(int type, int slotid, int error);
    oneway void oemHotswapSetInd(int type, int slotid);
    oneway void oemKeyLogErrInd(int type, in Oem_key_log_err_msg_type errMsg);
    oneway void oemLargeDataKeyLogErrInd(int type, in int[] result);
    oneway void oemLogPacketInd(int type, in Oem_log_packet_type log_packet_msg);
    oneway void oemPinPukRetryNumInd(int type, int slotid, int pin_retry_num, int puk_retry_num);
    oneway void smlDeviceLockInfoChangedInd(int type, String info);
    oneway void smlEncryptedSerialIdInd(int type, in String[] indStgs);
    oneway void subsidyDeviceLockInfoChangedInd(int type, String info);
    oneway void regionlockStatusChangedInd(int type, in byte[] region_lock_status_info);
    oneway void nrEvolutionTypeIndication(int type, int toe);
    oneway void singleSimConfigChangedInd(int type, int result);
    oneway void offlineLockStatusChangedInd(int type, in int[] status);
}
