//
// Created by seaking on 2020/7/27.
//

#ifndef FFMPEG_DEMO_NLOG_H
#define FFMPEG_DEMO_NLOG_H
#include <jni.h>
#include <android/log.h>

// log标签
#define TAG "NROM_DEBUG"
// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)


#endif //FFMPEG_DEMO_NLOG_H
