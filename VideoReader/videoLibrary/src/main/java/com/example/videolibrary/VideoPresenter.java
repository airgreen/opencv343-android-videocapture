package com.example.videolibrary;


import android.graphics.Bitmap;

public class VideoPresenter {

    final static String TAG = "VideoPresenter";

    private static VideoPresenter mInstance;


    static{
        System.loadLibrary("videoreader");
    }


    private VideoPresenter() {

    }

    public static VideoPresenter getInstance() {
        if (mInstance == null) {
            synchronized (VideoPresenter.class) {
                if (mInstance == null) {
                    mInstance = new VideoPresenter();
                }
            }
        }
        return mInstance;
    }


    public  void startReadVideo(final String videoPath,final int readFps){
        nativeStartReadVideo(videoPath,readFps);
    }


    public  void stopReadVideo(){
        nativeStopReadVideo();
    }


    public void startRecordVideo(String recordPath){
        nativeStartRecordVideo(recordPath);
    }


    public void stopRecordVideo(){
        nativeStopRecordVideo();
    }

    public void regVideoCallback(IVideoCallback videoCallback, Bitmap bitmap){
        nativeRegVideoCallback(videoCallback,bitmap);
    }


    public interface IVideoCallback{
        void onImageShow();
    }

    private static final native void nativeRegVideoCallback(IVideoCallback videoCallback,Bitmap bitmap);
    private static final native void nativeStartReadVideo(String videoPath,int readFps);
    private static final native void nativeStopReadVideo();
    private static final native void nativeStartRecordVideo(String recordPath);
    private static final native void nativeStopRecordVideo();

}
