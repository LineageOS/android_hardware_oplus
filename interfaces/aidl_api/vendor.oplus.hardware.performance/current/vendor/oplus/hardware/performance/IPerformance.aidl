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

package vendor.oplus.hardware.performance;
@VintfStability
interface IPerformance {
  int addAcmDirName(in String dirname, long flag);
  int addAcmNomediaDirName(in String dirname);
  int addAcmPkgName(in String pkgname, long flag);
  int addTaskTrackPid(int group, int pid, boolean clear);
  void clearTaskTrackGroup(int group);
  int delAcmDirName(in String dirname);
  int delAcmNomediaDirName(in String dirname);
  int delAcmPkgName(in String pkgname);
  int disableDamonReclaim();
  int disableKmallocDebug();
  int disableMultiThreadOptimize();
  int disableProcessReclaim();
  int disableTaskCpustats();
  int disableTaskPlacementDecision();
  int disableVmallocDebug();
  int enableAudioPerf(in String value);
  int enableDamonReclaim();
  int enableKmallocDebug();
  int enableMultiThreadOptimize();
  int enableProcessReclaim();
  int enableTaskCpustats();
  int enableTaskPlacementDecision();
  int enableVmallocDebug();
  int existMemMonitor();
  long getAcmDirFlag(in String dirname);
  int getAcmOpstat();
  long getAcmPkgFlag(in String pkgname);
  String getClmMuxSwitch();
  String getClmThreshold(int threshold_id);
  String getDdrResidency();
  String getDevinfoDDRInfo();
  String getDevinfoUfsInfo();
  String getDevinfoUfsVersionInfo();
  String getExt4FragScore(in String devpath);
  String getExt4FreefragInfo(in String devpath);
  String getF2fsMovedBlks();
  vendor.oplus.hardware.performance.ProcReqHal getHIAllocWait();
  String getHICpuInfo();
  String getHICpuLoading();
  vendor.oplus.hardware.performance.ProcReqHal getHIDState();
  vendor.oplus.hardware.performance.ProcReqHal getHIEmcdrvIowait();
  vendor.oplus.hardware.performance.ProcReqHal getHIFsyncWait();
  vendor.oplus.hardware.performance.ProcReqHal getHIIonWait();
  vendor.oplus.hardware.performance.ProcReqHal getHIIowait();
  vendor.oplus.hardware.performance.ProcReqHal getHIIowaitHung();
  vendor.oplus.hardware.performance.ProcReqHal getHIKswapdLoading();
  vendor.oplus.hardware.performance.ProcReqHal getHISchedLatency();
  vendor.oplus.hardware.performance.ProcReqHal getHIScmCall();
  vendor.oplus.hardware.performance.ProcReqHal getHIUfsFeature();
  int getKernelVersion();
  String getKmallocDebug();
  String getKmallocOrigin();
  String getKmallocUsed();
  String getMemMonitor();
  int getOswapVersion();
  String getUfsSignalRecordUpload();
  int getUfsplusHpbStatus();
  int getUfsplusTwStatus();
  String getVmallocDebug();
  String getVmallocHashCal();
  String getVmallocUsed();
  vendor.oplus.hardware.performance.TaskWakeInfo getWakeThreadsAffinityOrdered(in String handle, int size);
  String getallocwait();
  String getdstate();
  String getfsyncwait();
  String getionwait();
  String getiowait();
  String getschedlatency();
  String hybridswap_memcg_para_read(int action, in String cgroup);
  int hybridswap_memcg_para_write(int action, in String cgroup, in String str);
  String hybridswap_zram_para_read(int action);
  int hybridswap_zram_para_write(int action, in String str);
  boolean isJankTaskTrackEnable();
  int perProcessMemReadahead(int uid, int pid, int type);
  int perProcessMemReclaim(int uid, int pid, int type);
  String readCallStack();
  String readClmEnable();
  String readClmHighLoadAll();
  String readClmHighLoadGrp();
  String readClmLowLoadGrp();
  String readCpuTaskstats();
  String readDBacktrace();
  String readDConvert();
  String readFgFreqsThreshold();
  String readIOBacktrace();
  String readIomonitorInfo(in String procname);
  String readJankCpuIndicator();
  String readJankCpuInfo();
  String readJankCpuInfoSig();
  String readJankCpuLoad();
  String readJankCpuLoad32();
  String readJankCpuLoad32Scale();
  String readJankTaskTrack();
  String readJankTaskTrackByPid(int pid);
  String readJankVersion();
  String readKmallocDebugCreate();
  String readLimitTable();
  String readMemleakDetectThread();
  vendor.oplus.hardware.performance.ProcMemStatRet readMemoryByPids(in int[] pids, int flags);
  vendor.oplus.hardware.performance.ProcMemStatRet readMemoryByUids(in int[] uids, int flags);
  String readNandswapProc(in String inProc);
  String readNormalizeRealTime();
  String readNormalizeRunningTime();
  String readOplusReserve3(int offset, int len);
  vendor.oplus.hardware.performance.OsvelteVersionRet readOsvelteVersion();
  String readPidsSet();
  String readRealTime();
  String readRunningTime();
  String readSchedInfoThreshold();
  String readSgeFreqInfo();
  String readSgeInfo();
  String readStorageFeature(in String name, in String addr, in String isMulti);
  String readTargetProcess(in String buffer);
  String readTaskCpustatsEnable();
  String readTaskSchedInfo();
  String readTidsSet();
  String readTmemoryDirtypages();
  String readTmemoryErrorStat();
  String readTmemoryIoLatency();
  String readTmemorySysdirtypages();
  String readUxTaskTrack(int uPid, int rPid);
  String readVaFeature();
  String readVersion();
  void removeTaskTrackPid(int group, int pid);
  int searchAcmNomediaDirName(in String dirname);
  int setAcmOpstat(int flag);
  void setClmMuxSwitch(in String buffer);
  void setClmThreshold(in String buffer, int threshold_id);
  int setDamonReclaimColdTime(int cold_time);
  int setDamonReclaimMonitoring(int sample, int aggr);
  int setDamonReclaimQuota(int quota_ms, int quota_sz, int reset_interval);
  int setDamonReclaimWmarks(int metric, int high, int mid, int low);
  void setExtSchedProp(in String pid, in String prop);
  void setFgUids(in String fg_uid);
  void setFrameRate(in String frame_rate);
  int setFreqGoverner(in String gov_name, in int[] clusters);
  void setImFlag(in String pid, in String im_flag);
  int setProcessReclaim(in String info);
  void setSchedAssistImptTask(in String impt_info);
  void setSchedAssistScene(in String scene_id);
  void setSlideboost(in String boost);
  int setTpdID(in String param);
  int setTpdSerialParams(in String params);
  int setWakeSeedThread(in String tid, boolean identify_type, boolean inUid);
  void writeClmEnable(in String buffer);
  void writeClmHighLoadAll(in String buffer);
  void writeClmHighLoadGrp(in String buffer);
  void writeClmLowLoadGrp(in String buffer);
  void writeDBacktrace(in String buffer);
  void writeFgFreqsThreshold(in String buffer);
  void writeIOBacktrace(in String buffer);
  void writeJankTaskTrackEnable(boolean enable);
  int writeKmallocDebugCreate(int kcreate);
  int writeKmallocDebugCreateWithType(in String type);
  int writeMemMonitor(in String buffer);
  int writeMemleakDetectThread(int memdect);
  int writeMonitorStatus(in String buffer);
  int writeNandswapProc(in String inProc, in String cmd);
  int writeOplusReserve3(int offset, int len, in String info);
  void writePidsSet(in String buffer);
  void writeSchedInfoThreshold(in String buffer);
  int writeStorageFeature(in String name, in String addr, in String isMulti, in String cmd);
  void writeTaskSchedInfo(in String buffer);
  void writeTidsSet(in String buffer);
  int writeTmemoryCapacity(int param);
  int writeTmemoryFlushBusy(int param);
  int writeTmemoryFlushIdle(int param);
  int writeTmemoryHighWaterRatio(int param);
  int writeTmemoryMemory(in String str);
  int writeTmemorySwitch(int param);
  int writeUxState(in String ux_state, in String pid, in String tid);
  int writeVaFeature(int vafeature);
}
