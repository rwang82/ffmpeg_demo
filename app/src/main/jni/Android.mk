LOCAL_PATH := $(call my-dir)
MAIN_LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := Hello

LOCAL_CFLAGS := -Wno-error=format-security -fpermissive -DLOG_TAG=\"Hello\"
LOCAL_CFLAGS += -fno-rtti -fno-exceptions

LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)

LOCAL_SRC_FILES := com_daskks_ffmpeg_jniproxy_FFmpegJni.cpp

LOCAL_LDLIBS := -llog -latomic

include $(BUILD_SHARED_LIBRARY)