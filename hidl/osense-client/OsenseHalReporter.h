#pragma once

#include <vendor/oplus/hardware/osense/client/1.0/IOsenseHalReporter.h>

namespace vendor::oplus::hardware::osense::client::V1_0::implementation {

using android::hardware::hidl_string;
using android::hardware::Return;

struct OsenseHalReporter : public IOsenseHalReporter {
    Return<int32_t> checkAccessPermission(const hidl_string& identity) override;
    Return<void> osenseSetSceneAction(const hidl_string& identity,
                                      const OsenseHalSaRequest& request) override;
    Return<void> osenseClrSceneAction(const hidl_string& identity, int64_t request) override;
    Return<void> osenseSetNotification(const hidl_string& identity,
                                       const OsenseHalNotifyRequest& request) override;
    Return<void> osenseSetCtrlData(const hidl_string& identity,
                                   const OsenseControlInfo& ctrldata) override;
    Return<void> osenseResetCtrlData(const hidl_string& identity) override;
};

}
