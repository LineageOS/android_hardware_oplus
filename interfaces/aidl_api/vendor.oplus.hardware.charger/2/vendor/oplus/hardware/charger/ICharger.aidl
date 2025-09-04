/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
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

package vendor.oplus.hardware.charger;
@VintfStability
interface ICharger {
  int VolDividerIcWorkModeSet(String data);
  int chgExchangeMesgInit();
  int chgExchangeSohMesgInit();
  int getAcType();
  int getBattAuthenticate();
  int getBattPPSChgIng();
  int getBattPPSChgPower();
  String getBattParamNoplug();
  int getBattShortIcOtpStatus();
  int getBattSubCurrent();
  int getBattVoocChgIng();
  int getBatteryVoltageNow();
  String getBccCsvData();
  int getBccExpStatus();
  String getBmsHeatingRunningStatus();
  int getBmsHeatingStatus();
  String getChargerControl();
  int getChargerCoolDown();
  int getChargerCriticalLog();
  int getChargerIdVolt();
  int getChargerLog();
  int getCustomSelectChgMode();
  String getDevinfoFastchg();
  int getFastCharge();
  int getParallelChgMosTestResult();
  int getPsyAcOnline();
  int getPsyBatteryCC();
  int getPsyBatteryCurrentNow();
  int getPsyBatteryFcc();
  int getPsyBatteryHmac();
  int getPsyBatteryLevel();
  int getPsyBatteryNotify();
  int getPsyBatteryPchg();
  int getPsyBatteryPchgResetCount();
  int getPsyBatteryRm();
  int getPsyBatteryShortFeature();
  int getPsyBatteryShortStatus();
  String getPsyBatteryStatus();
  int getPsyBatteryTemp();
  int getPsyChargeTech();
  int getPsyFastChgType();
  int getPsyInputCurrent();
  int getPsyOtgOnline();
  int getPsyOtgSwitch();
  int getPsyPcPortOnline();
  int getPsyQGVbatDeviation();
  int getPsyTypeOrientation();
  int getPsyUsbOnline();
  int getPsyUsbStatus();
  String getPsyWirelessRX();
  String getPsyWirelessRxVersion();
  String getPsyWirelessTX();
  String getPsyWirelessTxVersion();
  int getQgVbatDeviation();
  String getQuickModeGain();
  String getReserveSocDebug();
  int getSmartChgMode();
  int getUIsohValue();
  String getUisohDebugParameterInfo();
  int getUsbInputCurrentNow();
  int getUsbPrimalType();
  int getWiredOtgOnline();
  int getWirelessAdapterPower();
  int getWirelessCapacity();
  int getWirelessChargePumpEn();
  int getWirelessCurrentNow();
  String getWirelessDeviated();
  int getWirelessOnline();
  int getWirelessPenPresent();
  int getWirelessPtmcId();
  int getWirelessRXEnable();
  int getWirelessRealType();
  String getWirelessTXEnable();
  int getWirelessUserSleepMode();
  int getWirelessVoltageNow();
  String healthd_update_ui_soc_decimal();
  int nightstandby(int status);
  int notifyScreenStatus(int status);
  String queryChargeInfo();
  int setChargeEMMode(String data);
  int setChargerControl(String data);
  int setChargerCoolDown(String data);
  int setChargerCriticalLog(String data);
  int setChargerCycle(String data);
  int setChargerFactoryModeTest(String data);
  int setChargerLog(String data);
  int setChgStatusToBcc(int status);
  int setCustomSelectChgMode(int mode, boolean enable);
  int setFastchgFwUpdate(String data);
  int setPsyMmiChgEn(String data);
  int setPsyOtgSwitch(String data);
  int setReserveSocDebug(String data);
  int setShipMode(String data);
  int setSmartChgMode(String data);
  int setSmartCoolDown(int coolDown, int normalCoolDown, String pkgName);
  int setTbattPwrOff(String data);
  int setUisohDebugInfo(String data);
  int setUsbPrimalType(String data);
  int setWirelessChargePumpEn(String data);
  int setWirelessFtmMode(String data);
  int setWirelessIconDelay(String data);
  int setWirelessIdtAdcTest(String data);
  int setWirelessPenSoc(String data);
  int setWirelessRXEnable(String data);
  int setWirelessTXEnable(String data);
  int setWirelessUserSleepMode(String data);
  int setWlsThirdPartitionInfo(String data);
  vendor.oplus.hardware.charger.testKitFeatureTestResult testKitFeatureTest(int index);
  String testKitGetFeatureList();
  String testKitGetFeatureName(int index);
  int testKitGetFeatureNum();
  int updateUiSohToPartion();
  String queryWlsPencilInfo();
  String getChgOlcConfig();
  int setChgOlcConfig(String data);
  int setSuperEnduranceStatus(String data);
  int setSuperEnduranceCount(String data);
  int setBobStatus(String data);
  int setPsySlowChgEn(String data);
  int getCpVbatDeviation();
  int setBatteryLogPush(String data);
  int getChargingModeInGsmCall();
  int setChargingModeInGsmCall(String data);
  int setChgRusConfig(String data);
}
