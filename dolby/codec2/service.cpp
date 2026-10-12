/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <dlfcn.h>
#include <cstdlib>
#include <memory>

#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <codec2/hidl/1.0/ComponentStore.h>
#include <hidl/HidlTransportSupport.h>

int main() {
    using android::hardware::media::c2::V1_0::utils::ComponentStore;

    android::ProcessState::initWithDriver("/dev/vndbinder");
    android::ProcessState::self()->startThreadPool();
    android::hardware::configureRpcThreadpool(8, true);

    // Keep the library loaded for the lifetime of the store and its components.
    void* library = dlopen("c2.dolby.store.so", RTLD_NOW | RTLD_LOCAL);
    if (library == nullptr) {
        LOG(ERROR) << "Cannot load Dolby component store: " << dlerror();
        return EXIT_FAILURE;
    }

    using GetStore = std::shared_ptr<C2ComponentStore> (*)();
    auto getStore = reinterpret_cast<GetStore>(
            dlsym(library, "_ZN7android28GetCodec2DolbyComponentStoreEv"));
    if (getStore == nullptr) {
        LOG(ERROR) << "Cannot find Dolby component store factory: " << dlerror();
        return EXIT_FAILURE;
    }

    auto implementation = getStore();
    if (implementation == nullptr) {
        LOG(ERROR) << "Dolby component store factory returned null";
        return EXIT_FAILURE;
    }

    auto service = android::sp<ComponentStore>::make(implementation);
    android::status_t status = service->registerAsService("dolby");
    if (status != android::OK) {
        LOG(ERROR) << "Cannot register Dolby Codec2 service: " << status;
        return EXIT_FAILURE;
    }

    android::hardware::joinRpcThreadpool();
    return EXIT_FAILURE;
}
