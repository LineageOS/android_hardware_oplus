#define LOG_TAG "vendor.oplus.hardware.osense.client@1.0-service"

#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>

#include "OsenseHalReporter.h"

using android::sp;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using vendor::oplus::hardware::osense::client::V1_0::IOsenseHalReporter;
using vendor::oplus::hardware::osense::client::V1_0::implementation::OsenseHalReporter;

int main() {
    sp<IOsenseHalReporter> service = new OsenseHalReporter();

    configureRpcThreadpool(1, true);

    if (service->registerAsService("OsenseHalReporter") != android::OK) {
        LOG(ERROR) << "Failed to register Osense HAL service";
        return 1;
    }

    joinRpcThreadpool();

    return 1;
}
