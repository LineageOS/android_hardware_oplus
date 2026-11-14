#include "OsenseHalReporter.h"

namespace vendor::oplus::hardware::osense::client::V1_0::implementation {

Return<int32_t> OsenseHalReporter::checkAccessPermission(const hidl_string&) {
    return 1;
}

Return<void> OsenseHalReporter::osenseSetSceneAction(const hidl_string&,
                                                     const OsenseHalSaRequest&) {
    return {};
}

Return<void> OsenseHalReporter::osenseClrSceneAction(const hidl_string&, int64_t) {
    return {};
}

Return<void> OsenseHalReporter::osenseSetNotification(const hidl_string&,
                                                      const OsenseHalNotifyRequest&) {
    return {};
}

Return<void> OsenseHalReporter::osenseSetCtrlData(const hidl_string&, const OsenseControlInfo&) {
    return {};
}

Return<void> OsenseHalReporter::osenseResetCtrlData(const hidl_string&) {
    return {};
}

}
