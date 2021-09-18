ifneq ($(filter $(call my-dir),$(PRODUCT_SOONG_NAMESPACES)),)

include $(call all-subdir-makefiles)

endif
