/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <string.h>

#include <iterator>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/stringprintf.h>
#include <cppbor.h>
#include <cppbor_parse.h>

using ::android::base::GetProperty;
using ::android::base::InitLogging;
using ::android::base::StringAppendF;

extern "C" {
// libminkdescriptor.so
int32_t TZCom_getClientEnvObject(void* out);
// libmodem_device_ids.so
int fetch_modem_device_ids(char* out);
}

namespace {

// Minimal MinkIPC object model (see QTI smcinvoke object.h)
struct Object;
typedef int32_t (*ObjectInvoke)(void* h, uint32_t op, void* args, uint32_t k);
struct Object {
    ObjectInvoke invoke;
    void* context;
};
struct ObjectBuf {
    const void* ptr;
    size_t size;
};
union ObjectArg {
    ObjectBuf b;
    Object o;
};

constexpr uint32_t kObjectOpRelease = 0xFFFF;

constexpr uint32_t ObjectCounts(uint32_t nBI, uint32_t nBO, uint32_t nOI, uint32_t nOO) {
    return nBI | (nBO << 4) | (nOI << 8) | (nOO << 12);
}

constexpr uint32_t kAppClientUid = 151;
constexpr uint32_t kKeyMintOpenId = 150;
constexpr char kKeyMintApp[] = "keymaster64";

constexpr uint32_t kCmdGetDeviceInfo = 0xFEAC;
constexpr uint32_t kCmdProvisionDeviceInfo = 0xFEAD;
constexpr size_t kRspBufSize = 0x8000;

// Owning Object reference, released on destruction
class ObjectRef {
  public:
    ObjectRef() = default;
    ObjectRef(const ObjectRef&) = delete;
    ObjectRef& operator=(const ObjectRef&) = delete;
    ObjectRef(ObjectRef&& other) noexcept : obj_(std::exchange(other.obj_, {})) {}
    ObjectRef& operator=(ObjectRef&& other) noexcept {
        if (this != &other) {
            reset();
            obj_ = std::exchange(other.obj_, {});
        }
        return *this;
    }
    ~ObjectRef() { reset(); }

    explicit operator bool() const { return obj_.invoke != nullptr; }

    void reset() {
        if (obj_.invoke) obj_.invoke(obj_.context, kObjectOpRelease, nullptr, 0);
        obj_ = {};
    }

    Object* put() {
        reset();
        return &obj_;
    }

    int32_t invoke(uint32_t op, ObjectArg* args, uint32_t k) {
        return obj_.invoke(obj_.context, op, args, k);
    }

    // op 0 with a single input buffer and a single output object,
    // returns the output object or the error code
    std::variant<ObjectRef, int32_t> open(const void* in, size_t inSize) {
        ObjectArg a[2] = {};
        a[0].b = {in, inSize};
        int32_t ret = invoke(0, a, ObjectCounts(1, 0, 0, 1));
        if (ret) return ret;
        ObjectRef out;
        *out.put() = a[1].o;
        return out;
    }

  private:
    Object obj_ = {};
};

ObjectRef getKeyMintObject() {
    ObjectRef env;
    int32_t ret = TZCom_getClientEnvObject(env.put());
    if (ret || !env) {
        LOG(ERROR) << "TZCom_getClientEnvObject failed: " << ret;
        return {};
    }

    uint32_t uid = kAppClientUid;
    auto appClient = env.open(&uid, sizeof(uid));
    if (auto* err = std::get_if<int32_t>(&appClient)) {
        LOG(ERROR) << "IClientEnv open(" << uid << ") failed: " << *err;
        return {};
    }

    auto app = std::get<ObjectRef>(appClient).open(kKeyMintApp, strlen(kKeyMintApp));
    if (auto* err = std::get_if<int32_t>(&app)) {
        LOG(ERROR) << "getAppObject(" << kKeyMintApp << ") failed: " << *err;
        return {};
    }

    uint32_t id = kKeyMintOpenId;
    auto km = std::get<ObjectRef>(app).open(&id, sizeof(id));
    if (auto* err = std::get_if<int32_t>(&km)) {
        LOG(ERROR) << "app open(" << id << ") failed: " << *err;
        return {};
    }
    return std::move(std::get<ObjectRef>(km));
}

struct ModemIds {
    std::string imei, imei2, meid;
};

ModemIds fetchModemIds() {
    ModemIds ids;
    // imei[15] @ 0, imei2[15] @ 15, meid[14] @ 30, not NUL terminated
    char buf[44] = {};
    int ret = fetch_modem_device_ids(buf);
    if (ret < 0) {
        LOG(ERROR) << "Fetching modem device ids failed: " << ret;
        return ids;
    }
    if (ret & 1) ids.imei.assign(buf, 15);
    if (ret & 2) ids.imei2.assign(buf + 15, 15);
    if (ret & 4) ids.meid.assign(buf + 30, 14);
    return ids;
}

// Sends a command to the TA, returns the response payload
std::optional<std::vector<uint8_t>> sendCmd(ObjectRef& km, const std::vector<uint8_t>& req) {
    std::vector<uint8_t> rsp(kRspBufSize);
    ObjectArg a[2] = {};
    a[0].b = {req.data(), req.size()};
    a[1].b = {rsp.data(), rsp.size()};
    int32_t ret = km.invoke(0, a, ObjectCounts(1, 1, 0, 0));
    if (ret) {
        LOG(ERROR) << "KeyMint TA invoke failed: " << ret;
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

using DeviceInfo = std::map<std::string, std::string>;

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

std::string formatDeviceInfo(const DeviceInfo& info) {
    std::string out;
    for (const auto& [key, value] : info) {
        bool sensitive = key == "serial" || key.rfind("imei", 0) == 0 || key.rfind("meid", 0) == 0;
        if (!out.empty()) out += "\n";
        out += "  " + key + ": ";
        if (sensitive) {
            out += value.empty() ? "<empty>" : "<redacted>";
        } else {
            out += value;
        }
    }
    return out;
}

// Fields compared by KmInstallKeybox (meid2 is not)
bool matchDeviceInfo(const DeviceInfo& a, const DeviceInfo& b) {
    for (const char* key : {"brand", "device", "product", "serial", "manufacturer", "model", "imei",
                            "imei2", "meid"}) {
        auto ia = a.find(key), ib = b.find(key);
        std::string va = ia == a.end() ? "" : ia->second;
        std::string vb = ib == b.end() ? "" : ib->second;
        if (va != vb) return false;
    }
    return true;
}

// Queries provisioned device info; returns nullopt if TA communication failed
std::optional<bool> getDeviceInfo(ObjectRef& km, DeviceInfo& info, std::string& err) {
    auto rsp = sendCmd(km, makeCmd(kCmdGetDeviceInfo));
    if (!rsp) return std::nullopt;
    bool ok = parseDeviceInfo(*rsp, info, err);
    if (!ok && err.rfind("malformed", 0) == 0) dumpHex("TA response", *rsp);
    return ok;
}

}  // namespace

int main(int argc, char** argv) {
    InitLogging(argv);

    bool force = argc == 3 && std::string(argv[2]) == "--force";
    if (argc != 2 && !force) {
        LOG(ERROR) << "Usage: " << argv[0] << " <serialno> [--force]";
        return 1;
    }

    ModemIds modem = fetchModemIds();

    DeviceInfo expected = {
            {"brand", GetProperty("ro.product.brand", "ro.product.brand unavailable")},
            {"device", GetProperty("ro.product.device", "ro.product.device unavailable")},
            {"product", GetProperty("ro.product.name", "ro.product.name unavailable")},
            {"serial", argv[1]},
            {"imei", modem.imei},
            {"imei2", modem.imei2},
            {"meid", modem.meid},
            {"manufacturer",
             GetProperty("ro.product.manufacturer", "ro.product.manufacturer unavailable")},
            {"model", GetProperty("ro.product.model", "ro.product.model unavailable")},
    };
    LOG(INFO) << "Device info:\n" << formatDeviceInfo(expected);

    ObjectRef km = getKeyMintObject();
    if (!km) return 1;

    DeviceInfo current;
    std::string err;
    auto provisioned = getDeviceInfo(km, current, err);
    if (!provisioned) return 1;

    if (*provisioned) {
        if (matchDeviceInfo(current, expected)) {
            LOG(INFO) << "Device IDs are already provisioned and match.";
            return 0;
        }
        LOG(WARNING) << "Device IDs are already provisioned and differ:\n"
                     << formatDeviceInfo(current);
        if (!force) {
            return 0;
        }
        LOG(INFO) << "Reprovisioning (--force).";
    } else {
        LOG(INFO) << "Device info not provisioned (" << err << "), provisioning.";
    }

    {
        // provisioning_wire::AttestationIdInfo, serialized as a CBOR map in struct field order
        cppbor::Map ids;
        for (const char* key : {"brand", "device", "product", "serial", "imei", "imei2", "meid",
                                "meid2", "manufacturer", "model"}) {
            if (auto it = expected.find(key); it != expected.end()) {
                ids.add(key, it->second);
            } else {
                ids.add(key, cppbor::Null());
            }
        }

        std::vector<uint8_t> req = makeCmd(kCmdProvisionDeviceInfo);
        ids.encode(std::back_inserter(req));

        // serde/ciborium encoded Result<(), String>
        auto rsp = sendCmd(km, req);
        if (!rsp) return 1;
        auto [item, pos, msg] = cppbor::parse(*rsp);
        const cppbor::Map* result = item ? item->asMap() : nullptr;
        if (!result || result->size() != 1 || !result->get("Ok")) {
            const auto& e = result ? result->get("Err") : item;
            if (result && e && e->asTstr()) {
                LOG(ERROR) << "Device info provisioning failed: " << e->asTstr()->value();
            } else {
                dumpHex("Device info provisioning failed, TA response", *rsp);
            }
            return 1;
        }
    }
    LOG(INFO) << "Device info provisioning complete.";

    current.clear();
    provisioned = getDeviceInfo(km, current, err);
    if (!provisioned) return 1;
    if (!*provisioned) {
        LOG(ERROR) << "Failed to read back device info: " << err;
        return 1;
    }
    if (!matchDeviceInfo(current, expected)) {
        LOG(ERROR) << "Provisioned device IDs differ:\n" << formatDeviceInfo(current);
        return 1;
    }
    LOG(INFO) << "Device IDs have been verified on the device.";
    return 0;
}
