# Shim library for missing symbols in libwvm
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CLANG := true

# only for 32bit libraries
LOCAL_SRC_FILES_32 := libwvmsym.cpp

LOCAL_SHARED_LIBRARIES := liblog libstagefright libcutils libmedia
LOCAL_MODULE := libwvmsym
LOCAL_MODULE_TAGS := optional
LOCAL_MULTILIB := 32

# crash! Disabled for the time being
#include $(BUILD_SHARED_LIBRARY)
