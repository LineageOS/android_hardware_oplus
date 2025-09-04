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

ndk::ScopedAStatus Performance::addAcmDirName(const std::string& dirname, int64_t flag,
                                              int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": dirname=" << dirname << ", flag=" << flag;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::addAcmNomediaDirName(const std::string& dirname,
                                                     int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": dirname=" << dirname;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::addAcmPkgName(const std::string& pkgname, int64_t flag,
                                              int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": pkgname=" << pkgname << ", flag=" << flag;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::addTaskTrackPid(int32_t group, int32_t pid, bool clear,
                                                int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": group=" << group << ", pid=" << pid << ", clear=" << clear;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::clearTaskTrackGroup(int32_t group) {
    LOG(INFO) << __func__ << ": group=" << group;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::delAcmDirName(const std::string& dirname, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": dirname=" << dirname;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::delAcmNomediaDirName(const std::string& dirname,
                                                     int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": dirname=" << dirname;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::delAcmPkgName(const std::string& pkgname, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": pkgname=" << pkgname;
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

ndk::ScopedAStatus Performance::enableAudioPerf(const std::string& value, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": value=" << value;
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

ndk::ScopedAStatus Performance::getAcmDirFlag(const std::string& dirname, int64_t* _aidl_return) {
    LOG(INFO) << __func__ << ": dirname=" << dirname;
    *_aidl_return = 0L;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getAcmOpstat(int32_t* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getAcmPkgFlag(const std::string& pkgname, int64_t* _aidl_return) {
    LOG(INFO) << __func__ << ": pkgname=" << pkgname;
    *_aidl_return = 0L;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getClmMuxSwitch(std::string* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getClmThreshold(int32_t threshold_id, std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": threshold_id=" << threshold_id;
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

ndk::ScopedAStatus Performance::getExt4FragScore(const std::string& devpath,
                                                 std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": devpath=" << devpath;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::getExt4FreefragInfo(const std::string& devpath,
                                                    std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": devpath=" << devpath;
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

ndk::ScopedAStatus Performance::getWakeThreadsAffinityOrdered(const std::string& handle,
                                                              int32_t size,
                                                              TaskWakeInfo* _aidl_return) {
    LOG(INFO) << __func__ << ": handle=" << handle << ", size=" << size;
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

ndk::ScopedAStatus Performance::hybridswap_memcg_para_read(int32_t action,
                                                           const std::string& cgroup,
                                                           std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": action=" << action << ", cgroup=" << cgroup;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::hybridswap_memcg_para_write(int32_t action,
                                                            const std::string& cgroup,
                                                            const std::string& str,
                                                            int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": action=" << action << ", cgroup=" << cgroup << ", str=" << str;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::hybridswap_zram_para_read(int32_t action,
                                                          std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": action=" << action;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::hybridswap_zram_para_write(int32_t action, const std::string& str,
                                                           int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": action=" << action << ", str=" << str;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::isJankTaskTrackEnable(bool* _aidl_return) {
    LOG(INFO) << __func__;
    *_aidl_return = false;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::perProcessMemReadahead(int32_t uid, int32_t pid, int32_t type,
                                                       int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": uid=" << uid << ", pid=" << pid << ", type=" << type;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::perProcessMemReclaim(int32_t uid, int32_t pid, int32_t type,
                                                     int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": uid=" << uid << ", pid=" << pid << ", type=" << type;
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

ndk::ScopedAStatus Performance::readIomonitorInfo(const std::string& procname,
                                                  std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": procname=" << procname;
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

ndk::ScopedAStatus Performance::readJankTaskTrackByPid(int32_t pid, std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": pid=" << pid;
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

ndk::ScopedAStatus Performance::readMemoryByPids(const std::vector<int32_t>& pids, int32_t flags,
                                                 ProcMemStatRet* _aidl_return) {
    LOG(INFO) << __func__ << ": pids.size=" << pids.size() << ", flags=" << flags;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readMemoryByUids(const std::vector<int32_t>& uids, int32_t flags,
                                                 ProcMemStatRet* _aidl_return) {
    LOG(INFO) << __func__ << ": uids.size=" << uids.size() << ", flags=" << flags;
    *_aidl_return = {};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readNandswapProc(const std::string& inProc,
                                                 std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": inProc=" << inProc;
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

ndk::ScopedAStatus Performance::readOplusReserve3(int32_t offset, int32_t len,
                                                  std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": offset=" << offset << ", len=" << len;
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

ndk::ScopedAStatus Performance::readStorageFeature(const std::string& name, const std::string& addr,
                                                   const std::string& isMulti,
                                                   std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": name=" << name << ", addr=" << addr << ", isMulti=" << isMulti;
    *_aidl_return = "";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::readTargetProcess(const std::string& buffer,
                                                  std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
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

ndk::ScopedAStatus Performance::readUxTaskTrack(int32_t uPid, int32_t rPid,
                                                std::string* _aidl_return) {
    LOG(INFO) << __func__ << ": uPid=" << uPid << ", rPid=" << rPid;
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

ndk::ScopedAStatus Performance::removeTaskTrackPid(int32_t group, int32_t pid) {
    LOG(INFO) << __func__ << ": group=" << group << ", pid=" << pid;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::searchAcmNomediaDirName(const std::string& dirname,
                                                        int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": dirname=" << dirname;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setAcmOpstat(int32_t flag, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": flag=" << flag;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setClmMuxSwitch(const std::string& buffer) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setClmThreshold(const std::string& buffer, int32_t threshold_id) {
    LOG(INFO) << __func__ << ": buffer=" << buffer << ", threshold_id=" << threshold_id;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setDamonReclaimColdTime(int32_t cold_time, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": cold_time=" << cold_time;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setDamonReclaimMonitoring(int32_t sample, int32_t aggr,
                                                          int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": sample=" << sample << ", aggr=" << aggr;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setDamonReclaimQuota(int32_t quota_ms, int32_t quota_sz,
                                                     int32_t reset_interval,
                                                     int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": quota_ms=" << quota_ms << ", quota_sz=" << quota_sz
              << ", reset_interval=" << reset_interval;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setDamonReclaimWmarks(int32_t metric, int32_t high, int32_t mid,
                                                      int32_t low, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": metric=" << metric << ", high=" << high << ", mid=" << mid
              << ", low=" << low;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setExtSchedProp(const std::string& pid, const std::string& prop) {
    LOG(INFO) << __func__ << ": pid=" << pid << ", prop=" << prop;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setFgUids(const std::string& fg_uid) {
    LOG(INFO) << __func__ << ": fg_uid=" << fg_uid;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setFrameRate(const std::string& frame_rate) {
    LOG(INFO) << __func__ << ": frame_rate=" << frame_rate;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setFreqGoverner(const std::string& gov_name,
                                                const std::vector<int32_t>& clusters,
                                                int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": gov_name=" << gov_name << ", clusters.size=" << clusters.size();
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setImFlag(const std::string& pid, const std::string& im_flag) {
    LOG(INFO) << __func__ << ": pid=" << pid << ", im_flag=" << im_flag;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setProcessReclaim(const std::string& info, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": info=" << info;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setSchedAssistImptTask(const std::string& impt_info) {
    LOG(INFO) << __func__ << ": impt_info=" << impt_info;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setSchedAssistScene(const std::string& scene_id) {
    LOG(INFO) << __func__ << ": scene_id=" << scene_id;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setSlideboost(const std::string& boost) {
    LOG(INFO) << __func__ << ": boost=" << boost;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setTpdID(const std::string& param, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": param=" << param;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setTpdSerialParams(const std::string& params,
                                                   int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": params=" << params;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::setWakeSeedThread(const std::string& tid, bool identify_type,
                                                  bool inUid, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": tid=" << tid << ", identify_type=" << identify_type
              << ", inUid=" << inUid;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeClmEnable(const std::string& buffer) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeClmHighLoadAll(const std::string& buffer) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeClmHighLoadGrp(const std::string& buffer) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeClmLowLoadGrp(const std::string& buffer) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeDBacktrace(const std::string& buffer) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeFgFreqsThreshold(const std::string& buffer) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeIOBacktrace(const std::string& buffer) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeJankTaskTrackEnable(bool enable) {
    LOG(INFO) << __func__ << ": enable=" << enable;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeKmallocDebugCreate(int32_t kcreate, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": kcreate=" << kcreate;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeKmallocDebugCreateWithType(const std::string& type,
                                                                int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": type=" << type;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeMemMonitor(const std::string& buffer, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeMemleakDetectThread(int32_t memdect, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": memdect=" << memdect;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeMonitorStatus(const std::string& buffer,
                                                   int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeNandswapProc(const std::string& inProc, const std::string& cmd,
                                                  int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": inProc=" << inProc << ", cmd=" << cmd;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeOplusReserve3(int32_t offset, int32_t len,
                                                   const std::string& info, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": offset=" << offset << ", len=" << len << ", info=" << info;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writePidsSet(const std::string& buffer) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeSchedInfoThreshold(const std::string& buffer) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeStorageFeature(const std::string& name,
                                                    const std::string& addr,
                                                    const std::string& isMulti,
                                                    const std::string& cmd, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": name=" << name << ", addr=" << addr << ", isMulti=" << isMulti
              << ", cmd=" << cmd;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTaskSchedInfo(const std::string& buffer) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTidsSet(const std::string& buffer) {
    LOG(INFO) << __func__ << ": buffer=" << buffer;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTmemoryCapacity(int32_t param, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": param=" << param;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTmemoryFlushBusy(int32_t param, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": param=" << param;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTmemoryFlushIdle(int32_t param, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": param=" << param;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTmemoryHighWaterRatio(int32_t param, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": param=" << param;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTmemoryMemory(const std::string& str, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": str=" << str;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeTmemorySwitch(int32_t param, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": param=" << param;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeUxState(const std::string& ux_state, const std::string& pid,
                                             const std::string& tid, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": ux_state=" << ux_state << ", pid=" << pid << ", tid=" << tid;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Performance::writeVaFeature(int32_t vafeature, int32_t* _aidl_return) {
    LOG(INFO) << __func__ << ": vafeature=" << vafeature;
    *_aidl_return = 0;
    return ndk::ScopedAStatus::ok();
}

}  // namespace performance
}  // namespace hardware
}  // namespace oplus
}  // namespace vendor
}  // namespace aidl
