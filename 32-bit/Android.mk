LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= program-32-bit-01.c

LOCAL_MODULE:= program-32-bit
LOCAL_32_BIT_ONLY := true

include $(BUILD_EXECUTABLE)
