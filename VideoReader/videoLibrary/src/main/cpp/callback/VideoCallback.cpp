#include <stdlib.h>
#include <linux/time.h>
#include <unistd.h>
#include <string.h>
#include <Util.h>
#include "VideoCallback.h"



VideoCallback::VideoCallback()
		:mVideoCallbackObj(NULL),mImageBitmapObj(NULL) {

	pthread_mutex_init(&callback_mutex, NULL);

}

VideoCallback::~VideoCallback() {

	pthread_mutex_destroy(&callback_mutex);

}


int VideoCallback::setCallback(JNIEnv *env, jobject callback_obj,jobject image_bitmap) {

	pthread_mutex_lock(&callback_mutex);
	{


		if (!env->IsSameObject(mVideoCallbackObj, callback_obj) || !env->IsSameObject(mImageBitmapObj, image_bitmap)) {
			ivideocallback_fields.onImageShow = NULL;

			if (mVideoCallbackObj) {
				env->DeleteGlobalRef(mVideoCallbackObj);
			}
			mVideoCallbackObj = callback_obj;


			if (mImageBitmapObj) {
				env->DeleteGlobalRef(mImageBitmapObj);
			}

			mImageBitmapObj = image_bitmap;

			if (callback_obj && image_bitmap) {
				// get method IDs of Java object for callback
				jclass clazz = env->GetObjectClass(callback_obj);
				if (clazz != NULL) {
					ivideocallback_fields.onImageShow = env->GetMethodID(clazz,
						"onImageShow",	"()V");


				} else {
					LOGI("failed to get object class");
				}
				env->ExceptionClear();
				if (!ivideocallback_fields.onImageShow) {
					LOGE("Can't find ivideocallback_fields#onImageShow");
					env->DeleteGlobalRef(callback_obj);
					mVideoCallbackObj = callback_obj = NULL;

					env->DeleteGlobalRef(mImageBitmapObj);
					mImageBitmapObj = image_bitmap = NULL;
				}
			}
		}
	}
	pthread_mutex_unlock(&callback_mutex);
    return  0;
}


//bitmap convert to cv mat image data
void bitmap_to_mat(JNIEnv *env, jobject &srcBitmap, cv::Mat &srcMat) {
	void *srcPixels = 0;
	AndroidBitmapInfo srcBitmapInfo;
	try {
		AndroidBitmap_getInfo(env, srcBitmap, &srcBitmapInfo);
		AndroidBitmap_lockPixels(env, srcBitmap, &srcPixels);
		uint32_t srcHeight = srcBitmapInfo.height;
		uint32_t srcWidth = srcBitmapInfo.width;
		srcMat.create(srcHeight, srcWidth, CV_8UC4);

		if (srcBitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
			cv::Mat tmp(srcHeight, srcWidth, CV_8UC4, srcPixels);
			tmp.copyTo(srcMat);
		} else {
			cv::Mat tmp = cv::Mat(srcHeight, srcWidth, CV_8UC2, srcPixels);
			cv::cvtColor(tmp, srcMat, CV_BGR5652RGBA);
		}

		AndroidBitmap_unlockPixels(env, srcBitmap);
		return;
	} catch (cv::Exception &e) {
		AndroidBitmap_unlockPixels(env, srcBitmap);
		jclass je = env->FindClass("java/lang/Exception");
		env -> ThrowNew(je, e.what());
		return;
	} catch (...) {
		AndroidBitmap_unlockPixels(env, srcBitmap);
		jclass je = env->FindClass("java/lang/Exception");
		env -> ThrowNew(je, "unknown");
		return;
	}
}


//cv mat image data convert to bitmap
void mat_to_bitmap(JNIEnv *env, cv::Mat &srcMat, jobject &dstBitmap) {
    void *dstPixels = 0;
    AndroidBitmapInfo dstBitmapInfo;
    try {
        AndroidBitmap_getInfo(env, dstBitmap, &dstBitmapInfo);
        AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);
        uint32_t dstHeight = dstBitmapInfo.height;
        uint32_t dstWidth = dstBitmapInfo.width;

        if (dstBitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            cv::Mat tmp(dstHeight, dstWidth, CV_8UC4, dstPixels);
            if(srcMat.type() == CV_8UC1) {
                cv::cvtColor(srcMat, tmp, CV_GRAY2RGBA);
            } else if (srcMat.type() == CV_8UC3) {
                cv::cvtColor(srcMat, tmp, CV_BGR2RGBA);
            } else if (srcMat.type() == CV_8UC4) {
                srcMat.copyTo(tmp);
            }
        } else {
            cv::Mat tmp = cv::Mat(dstHeight, dstWidth, CV_8UC2, dstPixels);
            if(srcMat.type() == CV_8UC1) {
                cv::cvtColor(srcMat, tmp, CV_GRAY2RGB);
            } else if (srcMat.type() == CV_8UC3) {
                cv::cvtColor(srcMat, tmp, CV_BGR5652RGB);
            } else if (srcMat.type() == CV_8UC4) {
                cv::cvtColor(srcMat, tmp, CV_BGRA2RGB);
            }
        }
        AndroidBitmap_unlockPixels(env, dstBitmap);
    }catch (cv::Exception &e) {
        AndroidBitmap_unlockPixels(env, dstBitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env -> ThrowNew(je, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(env, dstBitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env -> ThrowNew(je, "unknown");
        return;
    }
}

// show image
void VideoCallback::onImageShow(cv::Mat image) {

	JavaVM *vm = getVM();
	JNIEnv *env;
	// attach to JavaVM
	int attachedHere = 0; // know if detaching at the end is necessary
	JavaVMAttachArgs jvmArgs;
	jint res = vm->GetEnv((void**)&env, JNI_VERSION_1_6); // checks if current env needs attaching or it is already attached
	if (JNI_EDETACHED == res) {
		// Supported but not attached yet, needs to call AttachCurrentThread
		res = vm->AttachCurrentThread(reinterpret_cast<JNIEnv **>(&env), NULL);
		if (JNI_OK == res) {
			attachedHere = 1;
		} else {
			// Failed to attach, cancel
			return;
		}
	} else if (JNI_OK == res) {
		// Current thread already attached, do not attach 'again' (just to save the attachedHere flag)
		// We make sure to keep attachedHere = 0
	} else {
		// JNI_EVERSION, specified version is not supported cancel this..
		return;
	}

	pthread_mutex_lock(&callback_mutex);
	{
		if (mVideoCallbackObj) {

            mat_to_bitmap(env,image,mImageBitmapObj);
			env->CallVoidMethod(mVideoCallbackObj, ivideocallback_fields.onImageShow);

			env->ExceptionClear();
		}
	}

	pthread_mutex_unlock(&callback_mutex);

	if (attachedHere) { // Key check
		vm->DetachCurrentThread(); // Done only when attachment was done here
	}

}



