/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.oplus.hardware.communicationcenter;

import vendor.oplus.hardware.communicationcenter.DmtpFrameWrapper;
import vendor.oplus.hardware.communicationcenter.DmtpMessage;
import vendor.oplus.hardware.communicationcenter.TransportStats;

@VintfStability
interface IDmtpClient {
    oneway void onChannelAvailableChanged(boolean available);
    oneway void onFocusAudioDeviceChanged(int device);
    oneway void onPowerSaveModeChanged(boolean powerSaveMode, boolean ultraPowerSaveMode);
    oneway void onLocalTopicBusinessChanged(in byte[] localTopicBusiness);
    oneway void onRemoteTopicBusinessChanged(in byte[] remoteTopicBusiness);
    oneway void onTransporStatsChanged(in TransportStats stats);
    oneway void onVirtualCommCapabilityChanged(in boolean[] capability);
    oneway void receiveMessage(int sessionId, in DmtpMessage message);
    oneway void sentToNetwork(in byte[] data);
    oneway void sentToDev(int sessionId, in DmtpFrameWrapper frame);
}
