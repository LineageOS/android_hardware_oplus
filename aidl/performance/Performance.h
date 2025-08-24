/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/oplus/hardware/performance/BnPerformance.h>

namespace aidl {
namespace vendor {
namespace oplus {
namespace hardware {
namespace performance {

class Performance : public BnPerformance {
    ndk::ScopedAStatus addAcmDirName(const std::string& in_dirname, int64_t in_flag,
                                     int32_t* _aidl_return) override;
    ndk::ScopedAStatus addAcmNomediaDirName(const std::string& in_dirname,
                                            int32_t* _aidl_return) override;
    ndk::ScopedAStatus addAcmPkgName(const std::string& in_pkgname, int64_t in_flag,
                                     int32_t* _aidl_return) override;
    ndk::ScopedAStatus addTaskTrackPid(int32_t in_group, int32_t in_pid, bool in_clear,
                                       int32_t* _aidl_return) override;
    ndk::ScopedAStatus clearTaskTrackGroup(int32_t in_group) override;
    ndk::ScopedAStatus delAcmDirName(const std::string& in_dirname, int32_t* _aidl_return) override;
    ndk::ScopedAStatus delAcmNomediaDirName(const std::string& in_dirname,
                                            int32_t* _aidl_return) override;
    ndk::ScopedAStatus delAcmPkgName(const std::string& in_pkgname, int32_t* _aidl_return) override;
    ndk::ScopedAStatus disableDamonReclaim(int32_t* _aidl_return) override;
    ndk::ScopedAStatus disableKmallocDebug(int32_t* _aidl_return) override;
    ndk::ScopedAStatus disableMultiThreadOptimize(int32_t* _aidl_return) override;
    ndk::ScopedAStatus disableProcessReclaim(int32_t* _aidl_return) override;
    ndk::ScopedAStatus disableTaskCpustats(int32_t* _aidl_return) override;
    ndk::ScopedAStatus disableTaskPlacementDecision(int32_t* _aidl_return) override;
    ndk::ScopedAStatus disableVmallocDebug(int32_t* _aidl_return) override;
    ndk::ScopedAStatus enableAudioPerf(const std::string& in_value, int32_t* _aidl_return) override;
    ndk::ScopedAStatus enableDamonReclaim(int32_t* _aidl_return) override;
    ndk::ScopedAStatus enableKmallocDebug(int32_t* _aidl_return) override;
    ndk::ScopedAStatus enableMultiThreadOptimize(int32_t* _aidl_return) override;
    ndk::ScopedAStatus enableProcessReclaim(int32_t* _aidl_return) override;
    ndk::ScopedAStatus enableTaskCpustats(int32_t* _aidl_return) override;
    ndk::ScopedAStatus enableTaskPlacementDecision(int32_t* _aidl_return) override;
    ndk::ScopedAStatus enableVmallocDebug(int32_t* _aidl_return) override;
    ndk::ScopedAStatus existMemMonitor(int32_t* _aidl_return) override;
    ndk::ScopedAStatus getAcmDirFlag(const std::string& in_dirname, int64_t* _aidl_return) override;
    ndk::ScopedAStatus getAcmOpstat(int32_t* _aidl_return) override;
    ndk::ScopedAStatus getAcmPkgFlag(const std::string& in_pkgname, int64_t* _aidl_return) override;
    ndk::ScopedAStatus getClmMuxSwitch(std::string* _aidl_return) override;
    ndk::ScopedAStatus getClmThreshold(int32_t in_threshold_id, std::string* _aidl_return) override;
    ndk::ScopedAStatus getDdrResidency(std::string* _aidl_return) override;
    ndk::ScopedAStatus getDevinfoDDRInfo(std::string* _aidl_return) override;
    ndk::ScopedAStatus getDevinfoUfsInfo(std::string* _aidl_return) override;
    ndk::ScopedAStatus getDevinfoUfsVersionInfo(std::string* _aidl_return) override;
    ndk::ScopedAStatus getExt4FragScore(const std::string& in_devpath,
                                        std::string* _aidl_return) override;
    ndk::ScopedAStatus getExt4FreefragInfo(const std::string& in_devpath,
                                           std::string* _aidl_return) override;
    ndk::ScopedAStatus getF2fsMovedBlks(std::string* _aidl_return) override;
    ndk::ScopedAStatus getHIAllocWait(ProcReqHal* _aidl_return) override;
    ndk::ScopedAStatus getHICpuInfo(std::string* _aidl_return) override;
    ndk::ScopedAStatus getHICpuLoading(std::string* _aidl_return) override;
    ndk::ScopedAStatus getHIDState(ProcReqHal* _aidl_return) override;
    ndk::ScopedAStatus getHIEmcdrvIowait(ProcReqHal* _aidl_return) override;
    ndk::ScopedAStatus getHIFsyncWait(ProcReqHal* _aidl_return) override;
    ndk::ScopedAStatus getHIIonWait(ProcReqHal* _aidl_return) override;
    ndk::ScopedAStatus getHIIowait(ProcReqHal* _aidl_return) override;
    ndk::ScopedAStatus getHIIowaitHung(ProcReqHal* _aidl_return) override;
    ndk::ScopedAStatus getHIKswapdLoading(ProcReqHal* _aidl_return) override;
    ndk::ScopedAStatus getHISchedLatency(ProcReqHal* _aidl_return) override;
    ndk::ScopedAStatus getHIScmCall(ProcReqHal* _aidl_return) override;
    ndk::ScopedAStatus getHIUfsFeature(ProcReqHal* _aidl_return) override;
    ndk::ScopedAStatus getKernelVersion(int32_t* _aidl_return) override;
    ndk::ScopedAStatus getKmallocDebug(std::string* _aidl_return) override;
    ndk::ScopedAStatus getKmallocOrigin(std::string* _aidl_return) override;
    ndk::ScopedAStatus getKmallocUsed(std::string* _aidl_return) override;
    ndk::ScopedAStatus getMemMonitor(std::string* _aidl_return) override;
    ndk::ScopedAStatus getOswapVersion(int32_t* _aidl_return) override;
    ndk::ScopedAStatus getUfsSignalRecordUpload(std::string* _aidl_return) override;
    ndk::ScopedAStatus getUfsplusHpbStatus(int32_t* _aidl_return) override;
    ndk::ScopedAStatus getUfsplusTwStatus(int32_t* _aidl_return) override;
    ndk::ScopedAStatus getVmallocDebug(std::string* _aidl_return) override;
    ndk::ScopedAStatus getVmallocHashCal(std::string* _aidl_return) override;
    ndk::ScopedAStatus getVmallocUsed(std::string* _aidl_return) override;
    ndk::ScopedAStatus getWakeThreadsAffinityOrdered(const std::string& in_handle, int32_t in_size,
                                                     TaskWakeInfo* _aidl_return) override;
    ndk::ScopedAStatus getallocwait(std::string* _aidl_return) override;
    ndk::ScopedAStatus getdstate(std::string* _aidl_return) override;
    ndk::ScopedAStatus getfsyncwait(std::string* _aidl_return) override;
    ndk::ScopedAStatus getionwait(std::string* _aidl_return) override;
    ndk::ScopedAStatus getiowait(std::string* _aidl_return) override;
    ndk::ScopedAStatus getschedlatency(std::string* _aidl_return) override;
    ndk::ScopedAStatus hybridswap_memcg_para_read(int32_t in_action, const std::string& in_cgroup,
                                                  std::string* _aidl_return) override;
    ndk::ScopedAStatus hybridswap_memcg_para_write(int32_t in_action, const std::string& in_cgroup,
                                                   const std::string& in_str,
                                                   int32_t* _aidl_return) override;
    ndk::ScopedAStatus hybridswap_zram_para_read(int32_t in_action,
                                                 std::string* _aidl_return) override;
    ndk::ScopedAStatus hybridswap_zram_para_write(int32_t in_action, const std::string& in_str,
                                                  int32_t* _aidl_return) override;
    ndk::ScopedAStatus isJankTaskTrackEnable(bool* _aidl_return) override;
    ndk::ScopedAStatus perProcessMemReadahead(int32_t in_uid, int32_t in_pid, int32_t in_type,
                                              int32_t* _aidl_return) override;
    ndk::ScopedAStatus perProcessMemReclaim(int32_t in_uid, int32_t in_pid, int32_t in_type,
                                            int32_t* _aidl_return) override;
    ndk::ScopedAStatus readCallStack(std::string* _aidl_return) override;
    ndk::ScopedAStatus readClmEnable(std::string* _aidl_return) override;
    ndk::ScopedAStatus readClmHighLoadAll(std::string* _aidl_return) override;
    ndk::ScopedAStatus readClmHighLoadGrp(std::string* _aidl_return) override;
    ndk::ScopedAStatus readClmLowLoadGrp(std::string* _aidl_return) override;
    ndk::ScopedAStatus readCpuTaskstats(std::string* _aidl_return) override;
    ndk::ScopedAStatus readDBacktrace(std::string* _aidl_return) override;
    ndk::ScopedAStatus readDConvert(std::string* _aidl_return) override;
    ndk::ScopedAStatus readFgFreqsThreshold(std::string* _aidl_return) override;
    ndk::ScopedAStatus readIOBacktrace(std::string* _aidl_return) override;
    ndk::ScopedAStatus readIomonitorInfo(const std::string& in_procname,
                                         std::string* _aidl_return) override;
    ndk::ScopedAStatus readJankCpuIndicator(std::string* _aidl_return) override;
    ndk::ScopedAStatus readJankCpuInfo(std::string* _aidl_return) override;
    ndk::ScopedAStatus readJankCpuInfoSig(std::string* _aidl_return) override;
    ndk::ScopedAStatus readJankCpuLoad(std::string* _aidl_return) override;
    ndk::ScopedAStatus readJankCpuLoad32(std::string* _aidl_return) override;
    ndk::ScopedAStatus readJankCpuLoad32Scale(std::string* _aidl_return) override;
    ndk::ScopedAStatus readJankTaskTrack(std::string* _aidl_return) override;
    ndk::ScopedAStatus readJankTaskTrackByPid(int32_t in_pid, std::string* _aidl_return) override;
    ndk::ScopedAStatus readJankVersion(std::string* _aidl_return) override;
    ndk::ScopedAStatus readKmallocDebugCreate(std::string* _aidl_return) override;
    ndk::ScopedAStatus readLimitTable(std::string* _aidl_return) override;
    ndk::ScopedAStatus readMemleakDetectThread(std::string* _aidl_return) override;
    ndk::ScopedAStatus readMemoryByPids(const std::vector<int32_t>& in_pids, int32_t in_flags,
                                        ProcMemStatRet* _aidl_return) override;
    ndk::ScopedAStatus readMemoryByUids(const std::vector<int32_t>& in_uids, int32_t in_flags,
                                        ProcMemStatRet* _aidl_return) override;
    ndk::ScopedAStatus readNandswapProc(const std::string& in_inProc,
                                        std::string* _aidl_return) override;
    ndk::ScopedAStatus readNormalizeRealTime(std::string* _aidl_return) override;
    ndk::ScopedAStatus readNormalizeRunningTime(std::string* _aidl_return) override;
    ndk::ScopedAStatus readOplusReserve3(int32_t in_offset, int32_t in_len,
                                         std::string* _aidl_return) override;
    ndk::ScopedAStatus readOsvelteVersion(OsvelteVersionRet* _aidl_return) override;
    ndk::ScopedAStatus readPidsSet(std::string* _aidl_return) override;
    ndk::ScopedAStatus readRealTime(std::string* _aidl_return) override;
    ndk::ScopedAStatus readRunningTime(std::string* _aidl_return) override;
    ndk::ScopedAStatus readSchedInfoThreshold(std::string* _aidl_return) override;
    ndk::ScopedAStatus readSgeFreqInfo(std::string* _aidl_return) override;
    ndk::ScopedAStatus readSgeInfo(std::string* _aidl_return) override;
    ndk::ScopedAStatus readStorageFeature(const std::string& in_name, const std::string& in_addr,
                                          const std::string& in_isMulti,
                                          std::string* _aidl_return) override;
    ndk::ScopedAStatus readTargetProcess(const std::string& in_buffer,
                                         std::string* _aidl_return) override;
    ndk::ScopedAStatus readTaskCpustatsEnable(std::string* _aidl_return) override;
    ndk::ScopedAStatus readTaskSchedInfo(std::string* _aidl_return) override;
    ndk::ScopedAStatus readTidsSet(std::string* _aidl_return) override;
    ndk::ScopedAStatus readTmemoryDirtypages(std::string* _aidl_return) override;
    ndk::ScopedAStatus readTmemoryErrorStat(std::string* _aidl_return) override;
    ndk::ScopedAStatus readTmemoryIoLatency(std::string* _aidl_return) override;
    ndk::ScopedAStatus readTmemorySysdirtypages(std::string* _aidl_return) override;
    ndk::ScopedAStatus readUxTaskTrack(int32_t in_uPid, int32_t in_rPid,
                                       std::string* _aidl_return) override;
    ndk::ScopedAStatus readVaFeature(std::string* _aidl_return) override;
    ndk::ScopedAStatus readVersion(std::string* _aidl_return) override;
    ndk::ScopedAStatus removeTaskTrackPid(int32_t in_group, int32_t in_pid) override;
    ndk::ScopedAStatus searchAcmNomediaDirName(const std::string& in_dirname,
                                               int32_t* _aidl_return) override;
    ndk::ScopedAStatus setAcmOpstat(int32_t in_flag, int32_t* _aidl_return) override;
    ndk::ScopedAStatus setClmMuxSwitch(const std::string& in_buffer) override;
    ndk::ScopedAStatus setClmThreshold(const std::string& in_buffer,
                                       int32_t in_threshold_id) override;
    ndk::ScopedAStatus setDamonReclaimColdTime(int32_t in_cold_time,
                                               int32_t* _aidl_return) override;
    ndk::ScopedAStatus setDamonReclaimMonitoring(int32_t in_sample, int32_t in_aggr,
                                                 int32_t* _aidl_return) override;
    ndk::ScopedAStatus setDamonReclaimQuota(int32_t in_quota_ms, int32_t in_quota_sz,
                                            int32_t in_reset_interval,
                                            int32_t* _aidl_return) override;
    ndk::ScopedAStatus setDamonReclaimWmarks(int32_t in_metric, int32_t in_high, int32_t in_mid,
                                             int32_t in_low, int32_t* _aidl_return) override;
    ndk::ScopedAStatus setExtSchedProp(const std::string& in_pid,
                                       const std::string& in_prop) override;
    ndk::ScopedAStatus setFgUids(const std::string& in_fg_uid) override;
    ndk::ScopedAStatus setFrameRate(const std::string& in_frame_rate) override;
    ndk::ScopedAStatus setFreqGoverner(const std::string& in_gov_name,
                                       const std::vector<int32_t>& in_clusters,
                                       int32_t* _aidl_return) override;
    ndk::ScopedAStatus setImFlag(const std::string& in_pid, const std::string& in_im_flag) override;
    ndk::ScopedAStatus setProcessReclaim(const std::string& in_info,
                                         int32_t* _aidl_return) override;
    ndk::ScopedAStatus setSchedAssistImptTask(const std::string& in_impt_info) override;
    ndk::ScopedAStatus setSchedAssistScene(const std::string& in_scene_id) override;
    ndk::ScopedAStatus setSlideboost(const std::string& in_boost) override;
    ndk::ScopedAStatus setTpdID(const std::string& in_param, int32_t* _aidl_return) override;
    ndk::ScopedAStatus setTpdSerialParams(const std::string& in_params,
                                          int32_t* _aidl_return) override;
    ndk::ScopedAStatus setWakeSeedThread(const std::string& in_tid, bool in_identify_type,
                                         bool in_inUid, int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeClmEnable(const std::string& in_buffer) override;
    ndk::ScopedAStatus writeClmHighLoadAll(const std::string& in_buffer) override;
    ndk::ScopedAStatus writeClmHighLoadGrp(const std::string& in_buffer) override;
    ndk::ScopedAStatus writeClmLowLoadGrp(const std::string& in_buffer) override;
    ndk::ScopedAStatus writeDBacktrace(const std::string& in_buffer) override;
    ndk::ScopedAStatus writeFgFreqsThreshold(const std::string& in_buffer) override;
    ndk::ScopedAStatus writeIOBacktrace(const std::string& in_buffer) override;
    ndk::ScopedAStatus writeJankTaskTrackEnable(bool in_enable) override;
    ndk::ScopedAStatus writeKmallocDebugCreate(int32_t in_kcreate, int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeKmallocDebugCreateWithType(const std::string& in_type,
                                                       int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeMemMonitor(const std::string& in_buffer,
                                       int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeMemleakDetectThread(int32_t in_memdect, int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeMonitorStatus(const std::string& in_buffer,
                                          int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeNandswapProc(const std::string& in_inProc, const std::string& in_cmd,
                                         int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeOplusReserve3(int32_t in_offset, int32_t in_len,
                                          const std::string& in_info,
                                          int32_t* _aidl_return) override;
    ndk::ScopedAStatus writePidsSet(const std::string& in_buffer) override;
    ndk::ScopedAStatus writeSchedInfoThreshold(const std::string& in_buffer) override;
    ndk::ScopedAStatus writeStorageFeature(const std::string& in_name, const std::string& in_addr,
                                           const std::string& in_isMulti, const std::string& in_cmd,
                                           int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeTaskSchedInfo(const std::string& in_buffer) override;
    ndk::ScopedAStatus writeTidsSet(const std::string& in_buffer) override;
    ndk::ScopedAStatus writeTmemoryCapacity(int32_t in_param, int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeTmemoryFlushBusy(int32_t in_param, int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeTmemoryFlushIdle(int32_t in_param, int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeTmemoryHighWaterRatio(int32_t in_param, int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeTmemoryMemory(const std::string& in_str,
                                          int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeTmemorySwitch(int32_t in_param, int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeUxState(const std::string& in_ux_state, const std::string& in_pid,
                                    const std::string& in_tid, int32_t* _aidl_return) override;
    ndk::ScopedAStatus writeVaFeature(int32_t in_vafeature, int32_t* _aidl_return) override;
};

}  // namespace performance
}  // namespace hardware
}  // namespace oplus
}  // namespace vendor
}  // namespace aidl
