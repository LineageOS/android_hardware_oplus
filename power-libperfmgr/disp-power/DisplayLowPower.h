/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <string_view>

#include <android-base/unique_fd.h>

namespace aidl {
namespace google {
namespace hardware {
namespace power {
namespace impl {
namespace pixel {

class DisplayLowPower {
  public:
    DisplayLowPower();
    ~DisplayLowPower() {}
    void Init();
    void SetDisplayLowPower(bool enable);

  private:
    void ConnectPpsDaemon();
    int SendPpsCommand(const std::string_view cmd);
    void SetFoss(bool enable);

    ::android::base::unique_fd mPpsSocket;
    bool mFossStatus;
};

}  // namespace pixel
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace google
}  // namespace aidl
