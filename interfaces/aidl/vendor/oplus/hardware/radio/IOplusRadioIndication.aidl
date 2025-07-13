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
    void networkInfoInd(int type, in String[] networkinfo);
    void nvBackupStatusInd(int type, String result);
    void oemCallCssnfNumInd(int type, String cssNum);
    void oemEcclistInitialInd(int type, int status);
    void oemHookInd(in byte[] data);
    void oemHotswapProcessInd(int type, int slotid, int error);
    void oemHotswapSetInd(int type, int slotid);
    void oemKeyLogErrInd(int type, in Oem_key_log_err_msg_type errMsg);
    void oemLargeDataKeyLogErrInd(int type, in int[] result);
    void oemLogPacketInd(int type, in Oem_log_packet_type log_packet_msg);
    void oemLtdRecvInd(int type, int status);
    void oemLteCAInfoInd(int type, in int[] ca_info);
    void oemMsimSubModeIndication(int type, int msimsubMode, int dsdatxMode);
    void oemNonddsNullpagingInd(int type, in int[] dataInfos);
    void oemNoneDdsImsRegIndiction(in byte[] data);
    void oemPinPukRetryNumInd(int type, int slotid, int pin_retry_num, int puk_retry_num);
    void oemSimOutOfCreditInd(int type, int cmd_type, in byte[] data, int length);
    void smlDeviceLockInfoChangedInd(int type, String info);
    void smlEncryptedSerialIdInd(int type, in String[] indStgs);
    void subsidyDeviceLockInfoChangedInd(int type, String info);
    void regionlockStatusChangedInd(int type, in byte[] region_lock_status_info);
    void nrEvolutionTypeIndication(int type, int toe);
}
