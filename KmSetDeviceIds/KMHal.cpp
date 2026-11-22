/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "KMHal.h"

#include <string.h>

#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <cppbor.h>
#include <cppbor_parse.h>

using ::android::base::StringAppendF;

namespace {

// See libmink/SDK/QTVM/idl in https://github.com/qualcomm/minkipc
constexpr uint32_t CKMHal_UID = 150;
constexpr ObjectOp IKMHal_OP_sendCmd = 0;
constexpr ObjectOp IKMHal_OP_provisionDeviceIdsWithDualImei = 18;

constexpr char kKeyMintApp[] = "keymaster64";

constexpr uint32_t kCmdGetDeviceInfo = 0xFEAC;
// Status older (C) KeyMint TAs return for unknown commands
constexpr int32_t kStatusUnknownCmd = -40;
constexpr size_t kRspBufSize = 0x8000;

std::vector<uint8_t> makeCmd(uint32_t cmd) {
    std::vector<uint8_t> req(sizeof(cmd));
    memcpy(req.data(), &cmd, sizeof(cmd));
    return req;
}

void dumpHex(const char* what, const std::vector<uint8_t>& data) {
    std::string hex;
    for (uint8_t b : data) StringAppendF(&hex, " %02x", b);
    LOG(ERROR) << what << " (" << data.size() << " bytes):" << hex;
}

// Decodes serde/ciborium encoded Result<AttestationIdInfo, String>.
// Returns true and fills info on Ok, false and fills err on Err / decode failure.
bool parseDeviceInfo(const std::vector<uint8_t>& payload, DeviceInfo& info, std::string& err) {
    auto [item, pos, msg] = cppbor::parse(payload);
    const cppbor::Map* result = item ? item->asMap() : nullptr;
    if (!result || result->size() != 1) {
        err = "malformed response: " + msg;
        return false;
    }
    if (const auto& e = result->get("Err")) {
        err = e->asTstr() ? e->asTstr()->value() : "malformed error";
        return false;
    }
    const auto& ok = result->get("Ok");
    if (!ok || !ok->asMap()) {
        err = "malformed response: unexpected variant";
        return false;
    }
    for (const auto& [key, value] : *ok->asMap()) {
        if (!key->asTstr() || (!value->asTstr() && !value->asNull())) {
            err = "malformed device info";
            return false;
        }
        info[key->asTstr()->value()] = value->asTstr() ? value->asTstr()->value() : "";
    }
    return true;
}

}  // namespace

std::optional<KMHal> KMHal::open() {
    ObjectRef env;
    int32_t ret = TZCom_getClientEnvObject(env.put());
    if (ret || !env) {
        LOG(ERROR) << "TZCom_getClientEnvObject failed: " << ret;
        return std::nullopt;
    }

    uint32_t uid = CAppClient_UID;
    auto appClient = env.getObject(IClientEnv_OP_open, &uid, sizeof(uid));
    if (auto* err = std::get_if<int32_t>(&appClient)) {
        LOG(ERROR) << "IClientEnv_open(CAppClient_UID) failed: " << *err;
        return std::nullopt;
    }

    auto app = std::get<ObjectRef>(appClient).getObject(IAppClient_OP_getAppObject, kKeyMintApp,
                                                        strlen(kKeyMintApp));
    if (auto* err = std::get_if<int32_t>(&app)) {
        LOG(ERROR) << "IAppClient_getAppObject(" << kKeyMintApp << ") failed: " << *err;
        return std::nullopt;
    }

    uint32_t id = CKMHal_UID;
    auto km = std::get<ObjectRef>(app).getObject(IOpener_OP_open, &id, sizeof(id));
    if (auto* err = std::get_if<int32_t>(&km)) {
        LOG(ERROR) << "IOpener_open(CKMHal_UID) failed: " << *err;
        return std::nullopt;
    }
    return KMHal(std::move(std::get<ObjectRef>(km)));
}

int32_t KMHal::sendCmd(const std::vector<uint8_t>& req, std::vector<uint8_t>& rsp) {
    std::vector<uint8_t> buf(kRspBufSize);
    ObjectArg a[2] = {};
    a[0].bi = {req.data(), req.size()};
    a[1].b = {buf.data(), buf.size()};
    int32_t ret = km_.invoke(IKMHal_OP_sendCmd, a, ObjectCounts_pack(1, 1, 0, 0));
    if (ret) {
        LOG(ERROR) << "IKMHal_sendCmd failed: " << ret;
        return ret;
    }

    // u64 payload length followed by the payload. Older (C) TAs reply with an i32 status
    // followed by a u32 length instead, which shows up here as an invalid length.
    uint64_t len;
    memcpy(&len, buf.data(), sizeof(len));
    if (len > kRspBufSize - sizeof(len)) {
        int32_t status;
        memcpy(&status, buf.data(), sizeof(status));
        if (status < 0) return status;
        LOG(ERROR) << "Invalid TA response length: " << len;
        return -1;
    }
    rsp.assign(buf.begin() + sizeof(len), buf.begin() + sizeof(len) + len);
    return 0;
}

KMHal::QueryResult KMHal::getDeviceInfo(DeviceInfo& info, std::string& err) {
    std::vector<uint8_t> rsp;
    int32_t ret = sendCmd(makeCmd(kCmdGetDeviceInfo), rsp);
    if (ret == kStatusUnknownCmd) return QueryResult::kUnsupported;
    if (ret) {
        LOG(ERROR) << "Querying device info failed: " << ret;
        return QueryResult::kError;
    }
    if (parseDeviceInfo(rsp, info, err)) return QueryResult::kProvisioned;
    if (err.rfind("malformed", 0) == 0) dumpHex("TA response", rsp);
    return QueryResult::kNotProvisioned;
}

int32_t KMHal::provisionDeviceIds(const DeviceInfo& info) {
    // Order of IKMHal_provisionDeviceIdsWithDualImei arguments
    ObjectArg a[10] = {};
    size_t n = 0;
    for (const char* key : {"device", "brand", "model", "product", "manufacturer", "serial", "imei",
                            "imei2", "meid", "meid2"}) {
        // Missing IDs (e.g. meid2) are sent as empty buffers
        auto it = info.find(key);
        if (it != info.end()) a[n].bi = {it->second.data(), it->second.size()};
        n++;
    }
    return km_.invoke(IKMHal_OP_provisionDeviceIdsWithDualImei, a, ObjectCounts_pack(n, 0, 0, 0));
}
