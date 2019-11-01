LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := ThreadTest
LOCAL_MODULE_TAGS := eng debug
LOCAL_SRC_FILES := ThreadTest.cpp
LOCAL_SHARED_LIBRARIES := libutils
LOCAL_C_INCLUDES += system/core/include
include $(BUILD_EXECUTABLE)
