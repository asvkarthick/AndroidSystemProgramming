LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= program-64-bit-01.c

LOCAL_MODULE:= program-64-bit

include $(BUILD_EXECUTABLE)
