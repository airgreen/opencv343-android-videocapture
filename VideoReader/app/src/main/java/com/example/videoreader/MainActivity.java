package com.example.videoreader;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;

import com.example.videolibrary.VideoPresenter;
import com.example.videoreader.R;

public class MainActivity extends Activity {

    private final static String TAG = "MainActivity";


    private final static int WIDTH = 1280;
    private final static int HEIGHT = 720;

    private ImageView mImageView;
    private Button mBtnStartPlay;
    private Button mBtnStopPlay;
    private Button mBtnStartRecord;
    private Button mBtnStopRecord;


    private Bitmap mBitmap = null;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.activity_main);

        initView();
        regVideoCallback();

    }



    private void  initView(){

        mImageView = findViewById(R.id.iv_preview);
        mBtnStartPlay = findViewById(R.id.btn_start_play);
        mBtnStopPlay = findViewById(R.id.btn_stop_play);
        mBtnStartRecord = findViewById(R.id.btn_start_record);
        mBtnStopRecord = findViewById(R.id.btn_stop_record);

        mBtnStartPlay.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                VideoPresenter.getInstance().startReadVideo("/sdcard/testvideo.mp4",30);

            }
        });

        mBtnStopPlay.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                VideoPresenter.getInstance().stopReadVideo();
            }
        });


        mBtnStartRecord.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                VideoPresenter.getInstance().startRecordVideo("/sdcard/recordvideo.avi");

            }
        });


        mBtnStopRecord.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                VideoPresenter.getInstance().stopRecordVideo();

            }
        });
     }



    public void regVideoCallback(){

        mBitmap = Bitmap.createBitmap(WIDTH, HEIGHT, Bitmap.Config.ARGB_8888);
        VideoPresenter.getInstance().regVideoCallback(new VideoPresenter.IVideoCallback() {
            @Override
            public void onImageShow() {
                showImage();
            }

        }, mBitmap);

    }


    private void showImage(){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {

                mImageView.setImageBitmap(mBitmap);
            }
        });
    }




    @Override
    protected void onStart() {
        super.onStart();
    }

    @Override
    public void onPause() {
        super.onPause();
    }

    @Override
    public void onResume() {
        super.onResume();

        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED) {



        } else {
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.CAMERA,Manifest.permission.WRITE_EXTERNAL_STORAGE}, 100);
        }


    }




    @Override
    protected void onStop() {
        super.onStop();

    }

    @Override
    public void onDestroy() {
        super.onDestroy();


        if (mBitmap != null){
            mBitmap.recycle();
        }

    }




    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) {


            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setTitle("");
            builder.setMessage("您确定要退出APP?");
            builder.setPositiveButton("确定", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    finish();
                }
            });

            builder.setNegativeButton("取消", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                }
            });
            builder.show();
            return true;
        }

        return super.onKeyDown(keyCode, event);
    }

}
