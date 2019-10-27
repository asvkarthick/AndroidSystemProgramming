LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := BufferQueueTest
LOCAL_MODULE_TAGS := eng debug
LOCAL_SRC_FILES := BufferQueueTest.cpp
LOCAL_SHARED_LIBRARIES := libui libgui libutils libcutils libbinder
LOCAL_C_INCLUDES += frameworks/native/include system/core/include
include $(BUILD_EXECUTABLE)
