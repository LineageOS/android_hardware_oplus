/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */
///////////////////////////////////////////////////////////////////////////////
// THIS FILE IS IMMUTABLE. DO NOT EDIT IN ANY CASE.                          //
///////////////////////////////////////////////////////////////////////////////

// This file is a snapshot of an AIDL file. Do not edit it manually. There are
// two cases:
// 1). this is a frozen version file - do not edit this in any case.
// 2). this is a 'current' file. If you make a backwards compatible change to
//     the interface (from the latest frozen version), the build system will
//     prompt you to update this file with `m <name>-update-api`.
//
// You must not make a backward incompatible change to any AIDL file built
// with the aidl_interface module type with versions property set. The module
// type is used to build AIDL files in a way that they can be used across
// independently updatable components of the system. If a device is shipped
// with such a backward incompatible change, it has a high risk of breaking
// later when a module using the interface is updated, e.g., Mainline modules.

package vendor.oplus.hardware.communicationcenter;
@VintfStability
interface IDmtpService {
  int connect(String packageName, vendor.oplus.hardware.communicationcenter.IDmtpClient client);
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
  vendor.oplus.hardware.communicationcenter.TransportStats getTransporStats();
  void notifyPowerSaveMode(boolean powerSaveMode, boolean ultraPowerSaveMode);
  void notifyVirtualCommCapability(in boolean[] capability);
  void notifyFocusAudioDevice(int device);
  int sendMessage(int sessionId, in vendor.oplus.hardware.communicationcenter.DmtpMessage message, int flags);
  oneway void enableChannel(int channel, boolean enable);
  oneway void receiveFromNetwork(int sessionId, in byte[] data);
  oneway void receiveDmtpFrameFromNetwork(int sessionId, in vendor.oplus.hardware.communicationcenter.DmtpFrameWrapper frame);
  oneway void notifyDeviceState(int device, int state, boolean active, int reason);
  oneway void receiveFromDev(int sessionId, in vendor.oplus.hardware.communicationcenter.DmtpFrameWrapper frame);
}
