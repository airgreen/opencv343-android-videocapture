#ifndef UTIL_H
#define UTIL_H
#include <stdlib.h>
#include <unistd.h>
#include <jni.h>
#include <vector>
#include <android/log.h>
#include <mutex>

#define LOG_TAG "videolibrary-native"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


void setVM(JavaVM *);
JavaVM *getVM();
#endif  // UTIL_H
