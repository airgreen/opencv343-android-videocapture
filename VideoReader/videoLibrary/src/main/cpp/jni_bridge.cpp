#include <stdio.h>
#include "Util.h"
#include <chrono>
#include <thread>
#include <sys/prctl.h>
#include <opencv2/imgproc/types_c.h>
#include "include/opencv2/core/mat.hpp"
#include <include/opencv2/opencv.hpp>
#include <android/bitmap.h>
#include "VideoCallback.h"
#include "VideoReader.h"


std::string js2string(JNIEnv *env, jstring jStr) {
    const char *cstr = env->GetStringUTFChars(jStr, NULL);
    std::string str = std::string(cstr);
    env->ReleaseStringUTFChars(jStr, cstr);
    return str;
}

extern "C" {


std::shared_ptr<VideoReader> mVideoReaderPtr = nullptr;
VideoCallback* mVideoCallbackPtr;


void saveImage(cv::Mat cvImage){

    std::stringstream ss;
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    ss << timestamp << ".png";
    cv::imwrite("/sdcard/" + ss.str(), cvImage);
}


void showCVImage(cv::Mat cvImage){

    if (mVideoCallbackPtr != nullptr){
        mVideoCallbackPtr->onImageShow(cvImage);
    }
}

void videoReaderCallback(cv::Mat imageData){

    showCVImage(imageData);
//    saveImage(imageData);
}


void videoReaderRun(std::string video_path, int read_fps){

    prctl(PR_SET_NAME, "VideoReaderThread");
    
    mVideoReaderPtr->RegCallback(videoReaderCallback);
    mVideoReaderPtr->StartPlay(video_path,read_fps);
}





JNIEXPORT void JNICALL
Java_com_example_videolibrary_VideoPresenter_nativeStartReadVideo(JNIEnv *jniEnv, jobject obj,
                                                         jstring video_path, jint read_fps) {


    std::string path = js2string(jniEnv, video_path);

    LOGI("nativeReadVideo start video reader: videoPath - %s\n",path.c_str());
    if(mVideoReaderPtr == nullptr){
        mVideoReaderPtr = std::make_shared<VideoReader>();
    }

    std::thread thread_offline_video(videoReaderRun,path,read_fps);
    thread_offline_video.detach();

}


JNIEXPORT void JNICALL
Java_com_example_videolibrary_VideoPresenter_nativeStopReadVideo(JNIEnv *jniEnv, jobject obj
                                                                  ) {

    LOGI("nativeStopReadVideo ");

    if(mVideoReaderPtr != nullptr){
        mVideoReaderPtr->StopPlay();
    }
}


JNIEXPORT void JNICALL
Java_com_example_videolibrary_VideoPresenter_nativeStartRecordVideo(JNIEnv *jniEnv, jobject obj,
               jstring video_path){

    LOGI("nativeStartRecordVideo ");

    std::string path = js2string(jniEnv, video_path);
    if(mVideoReaderPtr == nullptr){
        mVideoReaderPtr = std::make_shared<VideoReader>();
    }

    mVideoReaderPtr->StartRecord(path);

}

JNIEXPORT void JNICALL
Java_com_example_videolibrary_VideoPresenter_nativeStopRecordVideo(JNIEnv *jniEnv, jobject obj
                                                                    ){

    LOGI("nativeStopRecordVideo ");

    if(mVideoReaderPtr != nullptr){
        mVideoReaderPtr->StopRecord();
    }


}

JNIEXPORT void JNICALL
Java_com_example_videolibrary_VideoPresenter_nativeRegVideoCallback(JNIEnv *jniEnv, jobject obj, jobject videoCallback, jobject bitmap
) {
    LOGI("nativeRegVideoCallback");

    if (mVideoCallbackPtr == nullptr) {
        mVideoCallbackPtr = new VideoCallback();

    }

    jobject video_callback_obj = jniEnv->NewGlobalRef(videoCallback);
    jobject videoBitmap = jniEnv->NewGlobalRef(bitmap);
    mVideoCallbackPtr->setCallback(jniEnv,video_callback_obj,videoBitmap);

}


jint JNI_OnLoad(JavaVM *vm, void *reserved) {


    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    setVM(vm);

    return JNI_VERSION_1_6;
}



}

