/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.subsys_interface.subsys;

import vendor.oplus.hardware.subsys_interface.subsys.IccStateInfo;
import vendor.oplus.hardware.subsys_interface.subsys.IccPerceptionInfo;
import vendor.oplus.hardware.subsys_interface.subsys.IccIndInfo;

@VintfStability
interface ISubsysIndication {
    void subsysStateChanged(int type, int state);
    void swtpStatusChanged(int type, byte status);
    void iccStateChanged(int type, in IccStateInfo state);
    void notifyIccPercStateChangedInd(int type, in IccPerceptionInfo state);
    void notifyIccInfoChanged(int type, in IccIndInfo info);
}
