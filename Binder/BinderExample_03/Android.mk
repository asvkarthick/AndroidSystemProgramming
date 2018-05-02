
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := binder_service_03
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := binder_service_03.cpp
LOCAL_SHARED_LIBRARIES := libutils libcutils libbinder
LOCAL_C_INCLUDES += frameworks/native/include system/core/include
LOCAL_C_INCLUDES += system/core/libcutils/include/cutils
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := binder_client_03
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := binder_client_03.cpp
LOCAL_SHARED_LIBRARIES := libutils libcutils libbinder
LOCAL_C_INCLUDES += frameworks/native/include system/core/include
LOCAL_C_INCLUDES += system/core/libcutils/include/cutils
include $(BUILD_EXECUTABLE)
