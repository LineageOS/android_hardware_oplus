/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#include <utility>
#include <variant>

// MinkIPC object model, see libminkadaptor/include/object.h in
// https://github.com/qualcomm/minkipc
typedef uint32_t ObjectOp;
typedef uint32_t ObjectCounts;
typedef void* ObjectCxt;
union ObjectArg;
typedef int32_t (*ObjectInvoke)(ObjectCxt h, ObjectOp op, ObjectArg* args, ObjectCounts counts);
struct Object {
    ObjectInvoke invoke;
    ObjectCxt context;
};
struct ObjectBuf {
    void* ptr;
    size_t size;
};
struct ObjectBufIn {
    const void* ptr;
    size_t size;
};
union ObjectArg {
    ObjectBuf b;
    ObjectBufIn bi;
    Object o;
};

constexpr ObjectOp Object_OP_release = 0xFFFF;

constexpr ObjectCounts ObjectCounts_pack(uint32_t nBuffersIn, uint32_t nBuffersOut,
                                         uint32_t nObjectsIn, uint32_t nObjectsOut) {
    return nBuffersIn | (nBuffersOut << 4) | (nObjectsIn << 8) | (nObjectsOut << 12);
}

// See libmink/SDK/QTVM/idl in https://github.com/qualcomm/minkipc
constexpr uint32_t CAppClient_UID = 0x97;
constexpr ObjectOp IClientEnv_OP_open = 0;
constexpr ObjectOp IAppClient_OP_getAppObject = 0;
constexpr ObjectOp IOpener_OP_open = 0;

extern "C" int32_t TZCom_getClientEnvObject(Object* out);

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
        if (obj_.invoke) obj_.invoke(obj_.context, Object_OP_release, nullptr, 0);
        obj_ = {};
    }

    Object* put() {
        reset();
        return &obj_;
    }

    int32_t invoke(ObjectOp op, ObjectArg* args, ObjectCounts counts) {
        return obj_.invoke(obj_.context, op, args, counts);
    }

    std::variant<ObjectRef, int32_t> getObject(ObjectOp op, const void* in, size_t inSize) {
        ObjectArg a[2] = {};
        a[0].bi = {in, inSize};
        int32_t ret = invoke(op, a, ObjectCounts_pack(1, 0, 0, 1));
        if (ret) return ret;
        ObjectRef out;
        *out.put() = a[1].o;
        return out;
    }

  private:
    Object obj_ = {};
};
