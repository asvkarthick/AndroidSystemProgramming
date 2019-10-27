LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := StreamSplitterTest
LOCAL_MODULE_TAGS := eng debug
LOCAL_SRC_FILES := StreamSplitterTest.cpp
LOCAL_SHARED_LIBRARIES := libui libgui libutils libcutils libbinder
LOCAL_C_INCLUDES += frameworks/native/include system/core/include
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := StreamSplitterMultiTest
LOCAL_MODULE_TAGS := eng debug
LOCAL_SRC_FILES := StreamSplitterMultiTest.cpp
LOCAL_SHARED_LIBRARIES := libui libgui libutils libcutils libbinder
LOCAL_C_INCLUDES += frameworks/native/include system/core/include
include $(BUILD_EXECUTABLE)
