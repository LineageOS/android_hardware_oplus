/*
 * SPDX-FileCopyrightText: 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ModemIds.h"

#include <stdint.h>
#include <string.h>

#include <memory>
#include <tuple>
#include <utility>

#include <android-base/logging.h>

extern "C" {
typedef struct qmi_client_struct* qmi_client_type;
typedef struct qmi_idl_service_object* qmi_idl_service_object_type;
int qmi_client_init_instance(qmi_idl_service_object_type service_obj, unsigned int instance_id,
                             void* ind_cb, void* ind_cb_data, void* os_params, unsigned int timeout,
                             qmi_client_type* user_handle);
int qmi_client_send_msg_sync(qmi_client_type user_handle, unsigned int msg_id, void* req_c_struct,
                             unsigned int req_c_struct_len, void* resp_c_struct,
                             unsigned int resp_c_struct_len, unsigned int timeout_msecs);
int qmi_client_release(qmi_client_type user_handle);

extern struct qmi_idl_service_object dms_qmi_idl_service_object_v01;
}

namespace {

// QMI DMS, see device_management_service_v01.h
constexpr unsigned int QMI_CLIENT_INSTANCE_ANY = 0xFFFF;
constexpr int QMI_TIMEOUT_ERR = -3;
constexpr unsigned int kQmiTimeoutMs = 5000;

constexpr unsigned int QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_REQ_V01 = 0x0025;
constexpr unsigned int QMI_DMS_BIND_SUBSCRIPTION_REQ_V01 = 0x0054;

constexpr int32_t DMS_PRIMARY_SUBS = 1;
constexpr int32_t DMS_SECONDARY_SUBS = 2;

constexpr size_t QMI_DMS_IMEI_LEN = 15;
constexpr size_t QMI_DMS_MEID_LEN = 14;

struct qmi_response_type_v01 {
    int32_t result;
    int32_t error;
};

struct dms_bind_subscription_req_msg_v01 {
    int32_t bind_subs;
};

struct dms_bind_subscription_resp_msg_v01 {
    qmi_response_type_v01 resp;
};

// Only the leading fields are used, newer IDL revisions append more (368 bytes in total
// for the one libqmiservices ships on sm8850), keep some headroom for the decoder.
struct dms_get_device_serial_numbers_resp_msg_v01 {
    qmi_response_type_v01 resp;
    uint8_t esn_valid;
    char esn[32 + 1];
    uint8_t imei_valid;
    char imei[32 + 1];
    uint8_t meid_valid;
    char meid[32 + 1];
    uint8_t reserved[1024];
};

using QmiClient = std::unique_ptr<qmi_client_struct, decltype(&qmi_client_release)>;

std::pair<std::string, std::string> getSerialNumbers(qmi_client_type client, int32_t subs) {
    dms_bind_subscription_req_msg_v01 bindReq = {subs};
    dms_bind_subscription_resp_msg_v01 bindResp = {};
    int ret = qmi_client_send_msg_sync(client, QMI_DMS_BIND_SUBSCRIPTION_REQ_V01, &bindReq,
                                       sizeof(bindReq), &bindResp, sizeof(bindResp), kQmiTimeoutMs);
    if (ret) {
        LOG(ERROR) << "QMI_DMS_BIND_SUBSCRIPTION(" << subs << ") failed: " << ret;
        return {};
    }

    dms_get_device_serial_numbers_resp_msg_v01 resp = {};
    ret = qmi_client_send_msg_sync(client, QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_REQ_V01, nullptr, 0,
                                   &resp, sizeof(resp), kQmiTimeoutMs);
    if (ret) {
        LOG(ERROR) << "QMI_DMS_GET_DEVICE_SERIAL_NUMBERS(" << subs << ") failed: " << ret;
        return {};
    }

    std::pair<std::string, std::string> ids;
    if (resp.imei_valid && strnlen(resp.imei, sizeof(resp.imei)) == QMI_DMS_IMEI_LEN) {
        ids.first.assign(resp.imei, QMI_DMS_IMEI_LEN);
    }
    if (resp.meid_valid && strnlen(resp.meid, sizeof(resp.meid)) == QMI_DMS_MEID_LEN) {
        ids.second.assign(resp.meid, QMI_DMS_MEID_LEN);
    }
    return ids;
}

}  // namespace

ModemIds fetchModemIds() {
    ModemIds ids;

    // qmi_client_os_params, zero initialized and not used for sync messages
    alignas(8) uint8_t osParams[128] = {};
    qmi_client_type handle = nullptr;
    int ret = qmi_client_init_instance(&dms_qmi_idl_service_object_v01, QMI_CLIENT_INSTANCE_ANY,
                                       nullptr, nullptr, osParams, kQmiTimeoutMs, &handle);
    if (ret == QMI_TIMEOUT_ERR) {
        LOG(WARNING) << "QMI DMS service not available";
        return ids;
    }
    if (ret) {
        LOG(ERROR) << "qmi_client_init_instance failed: " << ret;
        return ids;
    }
    QmiClient client(handle, qmi_client_release);

    std::tie(ids.imei, ids.meid) = getSerialNumbers(client.get(), DMS_PRIMARY_SUBS);
    ids.imei2 = getSerialNumbers(client.get(), DMS_SECONDARY_SUBS).first;
    return ids;
}
