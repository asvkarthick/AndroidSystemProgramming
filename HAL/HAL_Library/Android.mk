LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := hal_api.cpp

LOCAL_CFLAGS := -Wall -Wextra -Werror
LOCAL_CFLAGS += -DFDLEAK_FLAG
LOCAL_CFLAGS += -DMEMLEAK_FLAG

LOCAL_CFLAGS += -std=c++11 -std=gnu++1y

LOCAL_C_INCLUDES := system/media/camera/include \
                    hardware/libhardware/include/hardware

LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE := karthick_hal.default
LOCAL_MODULE_PATH_32 := $(TARGET_OUT_VENDOR)/lib

LOCAL_32_BIT_ONLY := true
include $(BUILD_SHARED_LIBRARY)
