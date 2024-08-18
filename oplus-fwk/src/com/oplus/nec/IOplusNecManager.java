/*
 * Copyright (C) 2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package com.oplus.nec;

import android.common.IOplusCommonFeature;
import android.common.OplusFeatureList;
import android.os.Bundle;

public interface IOplusNecManager extends IOplusCommonFeature {
    public static final IOplusNecManager DEFAULT = new IOplusNecManager() {};

    @Override
    default OplusFeatureList.OplusIndex index() {
        return OplusFeatureList.OplusIndex.IOplusNecManager;
    }

    @Override
    default IOplusCommonFeature getDefault() {
        return DEFAULT;
    }

    default void notifyNwDiagnoseInitComplete() {}
    default void broadcastNecEvent(int slotId, int eventId, Bundle data) {}
    default void broadcastServiceStateChanged(boolean oos, int slotId) {}
    default void broadcastDataConnect(int slotId, String type) {}
    default void broadcastDataConnectResult(int slotId, String type, boolean success) {}
    default void broadcastVolteVopsOrSettingChanged(int slotId, int event, boolean isVolteEnabled) {}
    default void broadcastDataDisconnect(int slotId, String type) {}
    default void broadcastDataDisconnectComplete(int slotId, String type) {}
    default void broadcastDataCallInternetProtocolType(int slotId, int protocol) {}
    default void broadcastNoDataIconError(int slotId, int errorcode, int protocol, String cause) {}
    default void broadcastNoDataFlowError(int slotId, int errorcode, String mNoDataFlowReason) {}
    default void broadcastNoDataFlowRecoveryError(int slotId, int errorcode, String recovery) {}
    default void broadcastSlowDataFlowError(int slotId, int errorcode, String score) {}
    default void broadcastSlowDataFlowRecovery(int slotId, int errorcode, String score) {}
    default void broadcastGameLargeDelayError(int slotId, int errorcode, String gameError) {}
    default void broadcastLimitState(int slotId, boolean limitState) {}
    default void broadcastPreferredNetworkMode(int slotId, int preferredMode) {}
    default void broadcastDataEnabledChanged(int slotId, boolean enabled) {}
    default void broadcastLostConnectionReason(int slotId, int errorcode, int lostReason) {}
    default void broadcastHangUpDelayTimer(int slotId, long millis, int csOrIms) {}
    default void broadcastCdmaResetActiveTimer(int slotId, int networkType) {}
    default void broadcastPreciseCallStateChanged(int slotId, int ring, int foreground, int background, int cause, int preciseCause, int disconnectState) {}
    default void broadcastSrvccStateChanged(int slotId, int srvccState) {}
    default void broadcastCallError(int slotId, int event, int cause, int preciseCause, String desc, boolean isImsCall, boolean isIncoming) {}
    default void broadcastVolteCallKeylog(int slotId, int event, String desc) {}
    default void broadcastImsRegisterState(int slotId, boolean imsRegisterState) {}
    default void broadcastRegInfoChanged(int voiceRegState, int voiceNetworkType, int dataRegState, int dataNetworkType, int slotId) {}
    default void broadcastImsNetworkStateChanged(int phoneId, int errorCode, String errorMessage, int regState, int imsRat) {}
    default void broadcastOnlineMeeting(int slotId, int errorcode, String record) {}
    default void broadcastDispatchedSmsId(int slotId, int errorCode, long messageId, boolean isWapPush, boolean isClass0, String mPackageName) {}
    default void broadcastDispatchedMoSmsId(int slotId, int eventId, String networkType, int rat, int errorType, int errorCode, String success) {}
}
