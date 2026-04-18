/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.communicationcenter;

import vendor.oplus.hardware.communicationcenter.DmtpFrameWrapper;
import vendor.oplus.hardware.communicationcenter.DmtpMessage;
import vendor.oplus.hardware.communicationcenter.IDmtpClient;
import vendor.oplus.hardware.communicationcenter.TransportStats;

@VintfStability
interface IDmtpService {
    int connect(String packageName, IDmtpClient client);
    int disConnect(int connectId);
    int registerAsChannel(int connectId);
    int unRegitsterChannel(int connectId);
    int openSession(int connectId, int businessId, String localTopic, String remoteTopic, boolean isServer, int reserve);
    int closeSession(int sessionId);
    int createTopic(String topic, boolean local, String business, String reserved);
    int removeTopic(String topic, String business);
    boolean isChannelAvailable();
    byte[] getLocalTopicBusiness();
    byte[] getRemoteTopicBusiness();
    boolean isPowerSaveMode();
    boolean[] getVirtualCommCapability();
    TransportStats getTransporStats();
    void notifyPowerSaveMode(boolean powerSaveMode, boolean ultraPowerSaveMode);
    void notifyVirtualCommCapability(in boolean[] capability);
    void notifyFocusAudioDevice(int device);
    int sendMessage(int sessionId, in DmtpMessage message, int flags);
    oneway void enableChannel(int channel, boolean enable);
    oneway void receiveFromNetwork(int sessionId, in byte[] data);
    oneway void receiveDmtpFrameFromNetwork(int sessionId, in DmtpFrameWrapper frame);
    oneway void notifyDeviceState(int device, int state, boolean active, int reason);
    oneway void receiveFromDev(int sessionId, in DmtpFrameWrapper frame);
}
