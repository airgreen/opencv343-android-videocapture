#pragma once

#include <string>
#include <chrono>
#include "../include/opencv2/core/mat.hpp"
#include "../include/opencv2/opencv.hpp"


typedef void (*FUN)(cv::Mat image_data);
class VideoReader {

public:
    VideoReader();

    void StartPlay(const std::string& video_path,int read_fps);
    void StopPlay();
    void StartRecord(const std::string& record_path);
    void StopRecord();
    void RegCallback(FUN callback);

private:
    std::shared_ptr<cv::VideoCapture> videoCap;
    std::shared_ptr<cv::VideoWriter> videoWriter;
    FUN videoCallback;
    std::string videoPath;
    std::string recordPath;
    int readFps{30};
    bool isPlay;
    bool isRecord;
};
