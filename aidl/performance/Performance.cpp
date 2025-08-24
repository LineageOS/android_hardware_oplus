/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Performance.h"

#include <android-base/logging.h>

#define LOG_TAG "vendor.oplus.hardware.performance-service"

namespace aidl {
namespace vendor {
namespace oplus {
namespace hardware {
namespace performance {

ndk::ScopedAStatus Performance::addAcmDirName(const std::string& in_dirname, int64_t in_flag,
                                              int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": dirname=" << in_dirname << ", flag=" << in_flag;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::addAcmNomediaDirName(const std::string& in_dirname,
                                                     int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": dirname=" << in_dirname;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::addAcmPkgName(const std::string& in_pkgname, int64_t in_flag,
                                              int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": pkgname=" << in_pkgname << ", flag=" << in_flag;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::addTaskTrackPid(int32_t in_group, int32_t in_pid, bool in_clear,
                                                int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": group=" << in_group << ", pid=" << in_pid << ", clear=" << in_clear;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::clearTaskTrackGroup(int32_t in_group) {
    LOG(INFO) << __func__ << ": group=" << in_group;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::delAcmDirName(const std::string& in_dirname,
                                              int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": dirname=" << in_dirname;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::delAcmNomediaDirName(const std::string& in_dirname,
                                                     int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": dirname=" << in_dirname;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::delAcmPkgName(const std::string& in_pkgname,
                                              int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": pkgname=" << in_pkgname;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::disableDamonReclaim(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::disableKmallocDebug(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::disableMultiThreadOptimize(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::disableProcessReclaim(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::disableTaskCpustats(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::disableTaskPlacementDecision(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::disableVmallocDebug(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::enableAudioPerf(const std::string& in_value,
                                                int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": value=" << in_value;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::enableDamonReclaim(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::enableKmallocDebug(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::enableMultiThreadOptimize(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::enableProcessReclaim(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::enableTaskCpustats(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::enableTaskPlacementDecision(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::enableVmallocDebug(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::existMemMonitor(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getAcmDirFlag(const std::string& in_dirname,
                                              int64_t* _aidl_return) {
    LOG(INFO) << __func__ << ": dirname=" << in_dirname;
    *_aidl_return = 0L;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getAcmOpstat(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getAcmPkgFlag(const std::string& in_pkgname,
                                              int64_t* _aidl_return) {
    LOG(INFO) << __func__ << ": pkgname=" << in_pkgname;
    *_aidl_return = 0L;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getClmMuxSwitch(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getClmThreshold(int32_t in_threshold_id,
                                                std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": threshold_id=" << in_threshold_id;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getDdrResidency(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getDevinfoDDRInfo(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getDevinfoUfsInfo(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getDevinfoUfsVersionInfo(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getExt4FragScore(const std::string& in_devpath,
                                                 std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": devpath=" << in_devpath;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getExt4FreefragInfo(const std::string& in_devpath,
                                                    std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": devpath=" << in_devpath;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getF2fsMovedBlks(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getHIAllocWait(ProcReqHal* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getHICpuInfo(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getHICpuLoading(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getHIDState(ProcReqHal* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getHIEmcdrvIowait(ProcReqHal* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getHIFsyncWait(ProcReqHal* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getHIIonWait(ProcReqHal* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getHIIowait(ProcReqHal* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getHIIowaitHung(ProcReqHal* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getHIKswapdLoading(ProcReqHal* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getHISchedLatency(ProcReqHal* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getHIScmCall(ProcReqHal* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getHIUfsFeature(ProcReqHal* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getKernelVersion(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getKmallocDebug(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getKmallocOrigin(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getKmallocUsed(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getMemMonitor(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getOswapVersion(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getUfsSignalRecordUpload(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getUfsplusHpbStatus(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getUfsplusTwStatus(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getVmallocDebug(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getVmallocHashCal(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getVmallocUsed(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getWakeThreadsAffinityOrdered(const std::string& in_handle,
                                                              int32_t in_size,
                                                              TaskWakeInfo* _aidl_return) {
    LOG(INFO) << __func__ << ": handle=" << in_handle << ", size=" << in_size;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getallocwait(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getdstate(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getfsyncwait(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getionwait(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getiowait(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getschedlatency(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::hybridswap_memcg_para_read(int32_t in_action,
                                                           const std::string& in_cgroup,
                                                           std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": action=" << in_action << ", cgroup=" << in_cgroup;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::hybridswap_memcg_para_write(int32_t in_action,
                                                            const std::string& in_cgroup,
                                                            const std::string& in_str,
                                                            int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": action=" << in_action << ", cgroup=" << in_cgroup
              << ", str=" << in_str;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::hybridswap_zram_para_read(int32_t in_action,
                                                          std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": action=" << in_action;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::hybridswap_zram_para_write(int32_t in_action,
                                                           const std::string& in_str,
                                                           int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": action=" << in_action << ", str=" << in_str;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::isJankTaskTrackEnable(bool* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = false;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::perProcessMemReadahead(int32_t in_uid, int32_t in_pid,
                                                       int32_t in_type, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": uid=" << in_uid << ", pid=" << in_pid << ", type=" << in_type;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::perProcessMemReclaim(int32_t in_uid, int32_t in_pid,
                                                     int32_t in_type, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": uid=" << in_uid << ", pid=" << in_pid << ", type=" << in_type;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readCallStack(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readClmEnable(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readClmHighLoadAll(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readClmHighLoadGrp(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readClmLowLoadGrp(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readCpuTaskstats(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readDBacktrace(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readDConvert(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readFgFreqsThreshold(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readIOBacktrace(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readIomonitorInfo(const std::string& in_procname,
                                                  std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": procname=" << in_procname;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readJankCpuIndicator(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readJankCpuInfo(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readJankCpuInfoSig(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readJankCpuLoad(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readJankCpuLoad32(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readJankCpuLoad32Scale(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readJankTaskTrack(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readJankTaskTrackByPid(int32_t in_pid, std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": pid=" << in_pid;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readJankVersion(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readKmallocDebugCreate(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readLimitTable(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readMemleakDetectThread(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readMemoryByPids(const std::vector<int32_t>& in_pids,
                                                 int32_t in_flags, ProcMemStatRet* _aidl_return) {
    LOG(INFO) << __func__ << ": pids.size=" << in_pids.size() << ", flags=" << in_flags;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readMemoryByUids(const std::vector<int32_t>& in_uids,
                                                 int32_t in_flags, ProcMemStatRet* _aidl_return) {
    LOG(INFO) << __func__ << ": uids.size=" << in_uids.size() << ", flags=" << in_flags;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readNandswapProc(const std::string& in_inProc,
                                                 std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": inProc=" << in_inProc;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readNormalizeRealTime(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readNormalizeRunningTime(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readOplusReserve3(int32_t in_offset, int32_t in_len,
                                                  std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": offset=" << in_offset << ", len=" << in_len;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readOsvelteVersion(OsvelteVersionRet* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readPidsSet(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readRealTime(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readRunningTime(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readSchedInfoThreshold(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readSgeFreqInfo(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readSgeInfo(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readStorageFeature(const std::string& in_name,
                                                   const std::string& in_addr,
                                                   const std::string& in_isMulti,
                                                   std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": name=" << in_name << ", addr=" << in_addr
              << ", isMulti=" << in_isMulti;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readTargetProcess(const std::string& in_buffer,
                                                  std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readTaskCpustatsEnable(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readTaskSchedInfo(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readTidsSet(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readTmemoryDirtypages(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readTmemoryErrorStat(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readTmemoryIoLatency(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readTmemorySysdirtypages(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readUxTaskTrack(int32_t in_uPid, int32_t in_rPid,
                                                std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": uPid=" << in_uPid << ", rPid=" << in_rPid;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readVaFeature(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readVersion(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::removeTaskTrackPid(int32_t in_group, int32_t in_pid) {
    LOG(INFO) << __func__ << ": group=" << in_group << ", pid=" << in_pid;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::searchAcmNomediaDirName(const std::string& in_dirname,
                                                        int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": dirname=" << in_dirname;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setAcmOpstat(int32_t in_flag, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": flag=" << in_flag;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setClmMuxSwitch(const std::string& in_buffer) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setClmThreshold(const std::string& in_buffer,
                                                int32_t in_threshold_id) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer << ", threshold_id=" << in_threshold_id;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setDamonReclaimColdTime(int32_t in_cold_time,
                                                        int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": cold_time=" << in_cold_time;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setDamonReclaimMonitoring(int32_t in_sample, int32_t in_aggr,
                                                          int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": sample=" << in_sample << ", aggr=" << in_aggr;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setDamonReclaimQuota(int32_t in_quota_ms, int32_t in_quota_sz,
                                                     int32_t in_reset_interval,
                                                     int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": quota_ms=" << in_quota_ms << ", quota_sz=" << in_quota_sz
              << ", reset_interval=" << in_reset_interval;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setDamonReclaimWmarks(int32_t in_metric, int32_t in_high,
                                                      int32_t in_mid, int32_t in_low,
                                                      int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": metric=" << in_metric << ", high=" << in_high << ", mid=" << in_mid
              << ", low=" << in_low;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setExtSchedProp(const std::string& in_pid,
                                                const std::string& in_prop) {
    LOG(INFO) << __func__ << ": pid=" << in_pid << ", prop=" << in_prop;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setFgUids(const std::string& in_fg_uid) {
    LOG(INFO) << __func__ << ": fg_uid=" << in_fg_uid;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setFrameRate(const std::string& in_frame_rate) {
    LOG(INFO) << __func__ << ": frame_rate=" << in_frame_rate;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setFreqGoverner(const std::string& in_gov_name,
                                                const std::vector<int32_t>& in_clusters,
                                                int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": gov_name=" << in_gov_name
              << ", clusters.size=" << in_clusters.size();
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setImFlag(const std::string& in_pid,
                                          const std::string& in_im_flag) {
    LOG(INFO) << __func__ << ": pid=" << in_pid << ", im_flag=" << in_im_flag;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setProcessReclaim(const std::string& in_info,
                                                  int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": info=" << in_info;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setSchedAssistImptTask(const std::string& in_impt_info) {
    LOG(INFO) << __func__ << ": impt_info=" << in_impt_info;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setSchedAssistScene(const std::string& in_scene_id) {
    LOG(INFO) << __func__ << ": scene_id=" << in_scene_id;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setSlideboost(const std::string& in_boost) {
    LOG(INFO) << __func__ << ": boost=" << in_boost;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setTpdID(const std::string& in_param, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": param=" << in_param;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setTpdSerialParams(const std::string& in_params,
                                                   int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": params=" << in_params;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setWakeSeedThread(const std::string& in_tid, bool in_identify_type,
                                                  bool in_inUid, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": tid=" << in_tid << ", identify_type=" << in_identify_type
              << ", inUid=" << in_inUid;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeClmEnable(const std::string& in_buffer) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeClmHighLoadAll(const std::string& in_buffer) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeClmHighLoadGrp(const std::string& in_buffer) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeClmLowLoadGrp(const std::string& in_buffer) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeDBacktrace(const std::string& in_buffer) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeFgFreqsThreshold(const std::string& in_buffer) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeIOBacktrace(const std::string& in_buffer) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeJankTaskTrackEnable(bool in_enable) {
    LOG(INFO) << __func__ << ": enable=" << in_enable;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeKmallocDebugCreate(int32_t in_kcreate, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": kcreate=" << in_kcreate;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeKmallocDebugCreateWithType(const std::string& in_type,
                                                                int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": type=" << in_type;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeMemMonitor(const std::string& in_buffer,
                                                int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeMemleakDetectThread(int32_t in_memdect,
                                                         int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": memdect=" << in_memdect;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeMonitorStatus(const std::string& in_buffer,
                                                   int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeNandswapProc(const std::string& in_inProc,
                                                  const std::string& in_cmd,
                                                  int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": inProc=" << in_inProc << ", cmd=" << in_cmd;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeOplusReserve3(int32_t in_offset, int32_t in_len,
                                                   const std::string& in_info,
                                                   int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": offset=" << in_offset << ", len=" << in_len << ", info=" << in_info;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writePidsSet(const std::string& in_buffer) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeSchedInfoThreshold(const std::string& in_buffer) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeStorageFeature(const std::string& in_name,
                                                    const std::string& in_addr,
                                                    const std::string& in_isMulti,
                                                    const std::string& in_cmd,
                                                    int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": name=" << in_name << ", addr=" << in_addr
              << ", isMulti=" << in_isMulti << ", cmd=" << in_cmd;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTaskSchedInfo(const std::string& in_buffer) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTidsSet(const std::string& in_buffer) {
    LOG(INFO) << __func__ << ": buffer=" << in_buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTmemoryCapacity(int32_t in_param, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": param=" << in_param;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTmemoryFlushBusy(int32_t in_param, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": param=" << in_param;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTmemoryFlushIdle(int32_t in_param, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": param=" << in_param;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTmemoryHighWaterRatio(int32_t in_param,
                                                           int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": param=" << in_param;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTmemoryMemory(const std::string& in_str,
                                                   int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": str=" << in_str;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTmemorySwitch(int32_t in_param, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": param=" << in_param;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeUxState(const std::string& in_ux_state,
                                             const std::string& in_pid, const std::string& in_tid,
                                             int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": ux_state=" << in_ux_state << ", pid=" << in_pid
              << ", tid=" << in_tid;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeVaFeature(int32_t in_vafeature, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": vafeature=" << in_vafeature;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

}  // namespace performance
}  // namespace hardware
}  // namespace oplus
}  // namespace vendor
}  // namespace aidl
