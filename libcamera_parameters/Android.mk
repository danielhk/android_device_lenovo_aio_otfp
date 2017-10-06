LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	CameraParameters.cpp

LOCAL_MODULE := libcamera_parameters_mtk

include $(BUILD_STATIC_LIBRARY)
