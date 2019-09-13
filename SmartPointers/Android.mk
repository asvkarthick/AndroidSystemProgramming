LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := strong_pointer
LOCAL_MODULE_TAGS := eng debug
LOCAL_SRC_FILES := strong_pointer.cpp
LOCAL_SHARED_LIBRARIES := libutils libcutils libbinder
LOCAL_C_INCLUDES += frameworks/native/include system/core/include
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := weak_pointer
LOCAL_MODULE_TAGS := eng debug
LOCAL_SRC_FILES := weak_pointer.cpp
LOCAL_SHARED_LIBRARIES := libutils libcutils libbinder
LOCAL_C_INCLUDES += frameworks/native/include system/core/include
include $(BUILD_EXECUTABLE)
