#ifndef VIDEOCALLBACK_H_
#define VIDEOCALLBACK_H_

#include <pthread.h>
#include <iostream>
#include <opencv2/imgproc/types_c.h>
#include "include/opencv2/core/mat.hpp"
#include <include/opencv2/opencv.hpp>
#include <android/bitmap.h>


#pragma interface

// for callback to Java object
typedef struct {

	jmethodID onImageShow;

} Fields_ivideocallback;

class VideoCallback {

private:
 	pthread_mutex_t callback_mutex;
 	jobject mVideoCallbackObj;
 	jobject mImageBitmapObj;
	Fields_ivideocallback ivideocallback_fields;

public:
	VideoCallback();
	~VideoCallback();

	void onImageShow(cv::Mat image);
	int setCallback(JNIEnv *env, jobject video_callback_obj, jobject image_bitmap);
};

#endif /* VIDEOCALLBACK_H_ */
