#include "VideoReader.h"
#include <chrono>
#include <Util.h>
#include <iostream>
#include <thread>
#include "Util.h"

using namespace std;


VideoReader::VideoReader()
            : videoPath(""), recordPath(""), videoCap(nullptr), videoWriter(nullptr), readFps(30), isPlay(false), isRecord(false){


}


std::chrono::time_point<std::chrono::steady_clock> last;

void VideoReader::StartPlay(const std::string &video_path, int read_fps) {

    videoPath = video_path;
    readFps = read_fps;

    isPlay = true;
    LOGI("VideoReader::StartPlay enter");
    LOGI("VideoReader::StartPlay %s",videoPath.c_str());

    videoCap = std::make_shared<cv::VideoCapture>(videoPath);

    if (!videoCap->isOpened()) {
        LOGI("open video file failed");
        return;
    }

//        double fps = videoCap->get(CV_CAP_PROP_FPS);
//        LOGI("video fps: %d",fps);

    int frameWidth = videoCap->get(CV_CAP_PROP_FRAME_WIDTH);
    int frameHeight = videoCap->get(CV_CAP_PROP_FRAME_HEIGHT);
    LOGI("video frameWidth: %d",frameWidth);
    LOGI("video frameHeight: %d",frameHeight);


    cv::Mat img;
    while (isPlay) {

        bool success = videoCap->read(img);
        if (!success) {
            LOGI("read frame failed");
            break;
        }

        if (img.empty()) {
            LOGI("frame is empty");
            break;
        }

        if (videoCallback != nullptr){
            videoCallback(img);
        }

        auto current = std::chrono::steady_clock::now();
        long duration = std::chrono::duration_cast<std::chrono::milliseconds>(current - last).count();
        last = current;

        LOGI("VideoReader::StartPlay read frame cost time: %dms  ",(int)duration);
        LOGI("VideoReader::StartPlay read frame fps: %f  ",(1000.0/duration*1.0));

        if (isRecord && recordPath != ""){

            if (videoWriter == nullptr){
                videoWriter = std::make_shared<cv::VideoWriter>(recordPath, CV_FOURCC('M', 'J', 'P', 'G'), 25, cv::Size(frameWidth, frameHeight));
            }

            videoWriter->write(img);
        }

        if (readFps > 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(1000/readFps));
        }

    }

    videoCap->release();
    LOGI("VideoReader::StartPlay exit");


}


void VideoReader::StopPlay() {
    isPlay = false;
}


void VideoReader::StopRecord() {
    isRecord = false;
}

void VideoReader::RegCallback(FUN callback){
    videoCallback = callback;
}

void VideoReader::StartRecord(const std::string& record_path) {
    isRecord = true;
    recordPath = record_path;
    LOGI("recordPath: %s",recordPath.c_str());

}

