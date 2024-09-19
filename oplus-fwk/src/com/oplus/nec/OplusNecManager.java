/*
 * Copyright (C) 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package com.oplus.nec;

import android.content.Context;
import android.os.Bundle;

public class OplusNecManager implements IOplusNecManager {
    private static OplusNecManager sInstance = null;

    public OplusNecManager(Context context) {}

    public static OplusNecManager getInstance(Context context) {
        if (sInstance == null) {
            sInstance = new OplusNecManager(context);
        }
        return sInstance;
    }

    public void notifyNwDiagnoseInitComplete() {}
    public void broadcastNecEvent(int slotId, int eventId, Bundle data) {}
    public void broadcastServiceStateChanged(boolean oos, int slotId) {}
    public void broadcastDataConnect(int slotId, String type) {}
    public void broadcastOlkKeylog(int slotId, Bundle data) {}
    public void broadcastDataConnectResult(int slotId, String type, boolean success) {}
    public void broadcastVolteVopsOrSettingChanged(int slotId, int event, boolean isVolteEnabled) {}
    public void broadcastDataDisconnect(int slotId, String type) {}
    public void broadcastDataDisconnectComplete(int slotId, String type) {}
    public void broadcastDataCallInternetProtocolType(int slotId, int protocol) {}
    public void broadcastNoDataIconError(int slotId, int errorcode, int protocol, String cause) {}
    public void broadcastNoDataFlowError(int slotId, int errorcode, String mNoDataFlowReason) {}
    public void broadcastNoDataFlowRecoveryError(int slotId, int errorcode, String recovery) {}
    public void broadcastFastRecoveryEvent(int slotId, int errorcode, String event) {}
    public void broadcastSlowDataFlowError(int slotId, int errorcode, String score) {}
    public void broadcastSlowDataFlowRecovery(int slotId, int errorcode, String score) {}
    public void broadcastGameLargeDelayError(int slotId, int errorcode, String gameError) {}
    public void broadcastLimitState(int slotId, boolean limitState) {}
    public void broadcastScanQrCodeStats(int slotId, int errorcode, String QrSuccess) {}
    public void broadcastPreferredNetworkMode(int slotId, int preferredMode) {}
    public void broadcastDataEnabledChanged(int slotId, boolean enabled) {}
    public void broadcastLostConnectionReason(int slotId, int errorcode, int lostReason) {}
    public void broadcastAnrEventStat(int slotId, int errorcode, String anrEventStat) {}
    public void broadcastHangUpDelayTimer(int slotId, long millis, int csOrIms) {}
    public void broadcastCdmaResetActiveTimer(int slotId, int networkType) {}
    public void broadcastPreciseCallStateChanged(int slotId, int ring, int foreground, int background, int cause, int preciseCause, int disconnectState) {}
    public void broadcastSrvccStateChanged(int slotId, int srvccState) {}
    public void broadcastCallError(int slotId, int event, int cause, int preciseCause, String desc, boolean isImsCall, boolean isIncoming) {}
    public void broadcastVolteCallKeylog(int slotId, int event, String desc) {}
    public void broadcastImsRegisterState(int slotId, boolean imsRegisterState) {}
    public void broadcastRegInfoChanged(int voiceRegState, int voiceNetworkType, int dataRegState, int dataNetworkType, int slotId) {}
    public void broadcastImsNetworkStateChanged(int phoneId, int errorCode, String errorMessage, int regState, int imsRat) {}
    public void broadcastScreenShare(int phoneId) {}
    public void broadcastScreenTouch(int phoneId, int screenTouchCount, int screenTouchSuccCount) {}
    public void broadcastOnlineMeeting(int slotId, int errorcode, String record) {}
    public void broadcastNetworkDiagnosis(int slotId, Bundle data) {}
    public void broadcastDispatchedSmsId(int slotId, int errorCode, long messageId, boolean isWapPush, boolean isClass0, String mPackageName) {}
    public void broadcastDispatchedMoSmsId(int slotId, int eventId, String networkType, int rat, int errorType, int errorCode, String success) {}
}
