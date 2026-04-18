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
parcelable DmtpConstants {
  const int BUSINESS_UNKNOWN = 0;
  const int BUSINESS_TEST = 1;
  const int BUSINESS_VIRTUALMODEM = 1;
  const int BUSINESS_RIL0 = 2;
  const int BUSINESS_RIL1 = 3;
  const int BUSINESS_RIL2 = 4;
  const int BUSINESS_IMSRIL0 = 5;
  const int BUSINESS_IMSRIL1 = 6;
  const int BUSINESS_DATASHARED = 7;
  const int BUSINESS_PHONE = 8;
  const int BUSINESS_COMMCENTER = 9;
  const int DTMP_VERSION = 1;
  const int DMTP_TOPIC_NEW = 1000;
  const int DMTP_TOPIC_PEER_ONLINE = 1001;
  const int DMTP_TOPIC_PEER_OFFLINE = 1002;
  const int DMTP_TOPIC_REMOVE = 1003;
  const int FOCUS_AUDIO_UNKNOWN = 0;
  const int FOCUS_AUDIO_PHONE = 1;
  const int FOCUS_AUDIO_PAD = 5;
  const byte MSG_TYPE_COMMAND = 1;
  const byte MSG_TYPE_REQUEST = 2;
  const byte MSG_TYPE_RESPONSE = 3;
  const byte MSG_TYPE_HEARTBEAT = 4;
  const byte MSG_TYPE_BROADCAST = 5;
  const int QOS_LOW_UM = 1;
  const int QOS_LOW_AM = 2;
  const int QOS_DEFAULT = 3;
  const int QOS_NORMAL_UM = 3;
  const int QOS_NORMAL_AM = 4;
  const int QOS_HIGH_UM = 5;
  const int QOS_HIGH_AM = 6;
  const int SESSION_TYPE_DEFAULT = 1;
  const int SESSION_TYPE_PROVIDER = 2;
  const int SYSTEM_MSG_ID_MIN = 64535;
  const int SYSTEM_MSG_CAPABILITY = 64536;
  const int SYSTEM_MSG_SCRRENSTATE = 64537;
  const int SYSTEM_MSG_TOPICBUSINESS = 64538;
  const int SYSTEM_MSG_HANDSHAKEREQ = 64539;
  const int SYSTEM_MSG_HANDSHAKERESP = 64540;
  const int SYSTEM_MSG_HANDSHAKEDONE = 64541;
  const int SYSTEM_MSG_STATESYNCREQ = 64542;
  const int SYSTEM_MSG_STATESYNRESP = 64543;
  const int SYSTEM_MSG_ID_MAX = 65535;
  const int TOPIC_VIRTUALCOMM = 1;
  const int TOPIC_TEST = 2;
  const int VIRTUALCOMM_CAPABILITY_ALL = 0;
  const int VIRTUALCOMM_CAPABILITY_CALLSHARED = 1;
  const int VIRTUALCOMM_CAPABILITY_SMSSHARED = 2;
  const int VIRTUALCOMM_CAPABILITY_DATASHARED = 3;
  const int VIRTUALCOMM_CAPABILITY_MAX = 4;
  const String TEST_TOPIC = "test";
  const String TEST_BUSINESS = "testbusiness";
  const String VIRTUALCOMM_COMMCENTER = "commcenter";
  const String VIRTUALCOMM_DATASHRED = "datashred";
  const String VIRTUALCOMM_IMSRIL0 = "imsril0";
  const String VIRTUALCOMM_IMSRIL1 = "imsril1";
  const String VIRTUALCOMM_PHONE = "phone";
  const String VIRTUALCOMM_RIL0 = "ril0";
  const String VIRTUALCOMM_RIL1 = "ril1";
  const String VIRTUALCOMM_RIL2 = "ril2";
  const String VIRTUALCOMM_TOPIC = "virtualcomm";
  const String VIRTUALCOMM_VM = "vmodem";
}
