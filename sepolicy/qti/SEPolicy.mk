#
# Copyright (C) 2022-2023 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#
ifeq ($(SEPOLICY_PATH), device/qcom/sepolicy_vndr/legacy-um)
    SEPOLICY_PLATFORM_COMMON := legacy-um
else
    SEPOLICY_PLATFORM_COMMON := common-um
    ifeq ($(SEPOLICY_PATH), device/qcom/sepolicy_vndr/sm8450)
        SEPOLICY_PLATFORM_SPECIFIC := sm8450
    else ifeq ($(SEPOLICY_PATH), device/qcom/sepolicy_vndr/sm8550)
        SEPOLICY_PLATFORM_SPECIFIC := sm8550
    endif
endif

BOARD_VENDOR_SEPOLICY_DIRS += \
    hardware/oplus/sepolicy/qti/vendor \
    hardware/oplus/sepolicy/qti/vendor/$(SEPOLICY_PLATFORM_COMMON)

SYSTEM_EXT_PRIVATE_SEPOLICY_DIRS += \
    hardware/oplus/sepolicy/qti/private \
    hardware/oplus/sepolicy/qti/private/$(SEPOLICY_PLATFORM_COMMON)

SYSTEM_EXT_PUBLIC_SEPOLICY_DIRS += \
    hardware/oplus/sepolicy/qti/public \
    hardware/oplus/sepolicy/qti/public/$(SEPOLICY_PLATFORM_COMMON)

ifneq ($(SEPOLICY_PLATFORM_SPECIFIC),)
BOARD_VENDOR_SEPOLICY_DIRS += \
    hardware/oplus/sepolicy/qti/vendor/$(SEPOLICY_PLATFORM_SPECIFIC)

SYSTEM_EXT_PRIVATE_SEPOLICY_DIRS += \
    hardware/oplus/sepolicy/qti/private/$(SEPOLICY_PLATFORM_SPECIFIC)

SYSTEM_EXT_PUBLIC_SEPOLICY_DIRS += \
    hardware/oplus/sepolicy/qti/public/$(SEPOLICY_PLATFORM_SPECIFIC)
endif
