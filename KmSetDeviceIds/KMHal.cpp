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
std::variant<DeviceInfo, std::string> parseDeviceInfo(const std::vector<uint8_t>& payload) {
    auto [item, pos, msg] = cppbor::parse(payload);
    const cppbor::Map* result = item ? item->asMap() : nullptr;
    if (!result || result->size() != 1) {
        return "malformed response: " + msg;
    }
    if (const auto& e = result->get("Err")) {
        return e->asTstr() ? e->asTstr()->value() : "malformed error";
    }
    const auto& ok = result->get("Ok");
    if (!ok || !ok->asMap()) {
        return "malformed response: unexpected variant";
    }
    DeviceInfo info;
    for (const auto& [key, value] : *ok->asMap()) {
        if (!key->asTstr() || (!value->asTstr() && !value->asNull())) {
            return "malformed device info";
        }
        if (std::string* field = info.field(key->asTstr()->value())) {
            *field = value->asTstr() ? value->asTstr()->value() : "";
        }
    }
    return info;
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

std::optional<std::vector<uint8_t>> KMHal::sendCmd(const std::vector<uint8_t>& req) {
    std::vector<uint8_t> rsp(kRspBufSize);
    ObjectArg a[2] = {};
    a[0].bi = {req.data(), req.size()};
    a[1].b = {rsp.data(), rsp.size()};
    int32_t ret = km_.invoke(IKMHal_OP_sendCmd, a, ObjectCounts_pack(1, 1, 0, 0));
    if (ret) {
        LOG(ERROR) << "IKMHal_sendCmd failed: " << ret;
        return std::nullopt;
    }

    // u64 payload length followed by the payload
    uint64_t len;
    memcpy(&len, rsp.data(), sizeof(len));
    if (len > kRspBufSize - sizeof(len)) {
        LOG(ERROR) << "Invalid TA response length: " << len;
        return std::nullopt;
    }
    return std::vector<uint8_t>(rsp.begin() + sizeof(len), rsp.begin() + sizeof(len) + len);
}

std::optional<std::variant<DeviceInfo, std::string>> KMHal::getDeviceInfo() {
    auto rsp = sendCmd(makeCmd(kCmdGetDeviceInfo));
    if (!rsp) return std::nullopt;
    auto result = parseDeviceInfo(*rsp);
    if (auto* err = std::get_if<std::string>(&result); err && err->rfind("malformed", 0) == 0) {
        dumpHex("TA response", *rsp);
    }
    return result;
}

int32_t KMHal::provisionDeviceIds(const DeviceInfo& info) {
    ObjectArg a[] = {
            {.bi = {info.device.data(), info.device.size()}},
            {.bi = {info.brand.data(), info.brand.size()}},
            {.bi = {info.model.data(), info.model.size()}},
            {.bi = {info.product.data(), info.product.size()}},
            {.bi = {info.manufacturer.data(), info.manufacturer.size()}},
            {.bi = {info.serial.data(), info.serial.size()}},
            {.bi = {info.imei.data(), info.imei.size()}},
            {.bi = {info.imei2.data(), info.imei2.size()}},
            {.bi = {info.meid.data(), info.meid.size()}},
            {.bi = {info.meid2.data(), info.meid2.size()}},
    };
    return km_.invoke(IKMHal_OP_provisionDeviceIdsWithDualImei, a,
                      ObjectCounts_pack(std::size(a), 0, 0, 0));
}
