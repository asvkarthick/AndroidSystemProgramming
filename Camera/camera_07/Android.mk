LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := camera_07.cpp

LOCAL_CFLAGS := -Wall -Wextra -Werror
LOCAL_CFLAGS += -DFDLEAK_FLAG
LOCAL_CFLAGS += -DMEMLEAK_FLAG

LOCAL_CFLAGS += -std=c++11 -std=gnu++1y

LOCAL_C_INCLUDES := system/media/camera/include \
                    hardware/libhardware/include/hardware

LOCAL_SHARED_LIBRARIES := libhardware

LOCAL_MODULE := camera_07

LOCAL_32_BIT_ONLY := true
include $(BUILD_EXECUTABLE)
