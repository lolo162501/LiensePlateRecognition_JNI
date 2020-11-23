package com.example.lienserecognition_opencv_demo;

import android.os.Bundle;
import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraActivity;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.JavaCamera2View;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends CameraActivity implements CameraBridgeViewBase.CvCameraViewListener2 {
    private JavaCamera2View javaCameraView;
    private Mat inputImage;

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        javaCameraView = findViewById(R.id.javaCameraView);
    }

    @Override
    protected void onResume() {
        if (OpenCVLoader.initDebug()) {
            baseLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        } else {
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION, this, baseLoaderCallback);
        }
        super.onResume();
    }

    @Override
    public void onPause() {
        super.onPause();
        if (javaCameraView != null) {
            javaCameraView.disableView();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (javaCameraView != null) {
            javaCameraView.disableView();
        }
    }

    private BaseLoaderCallback baseLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            if (status == LoaderCallbackInterface.SUCCESS) {
                if (javaCameraView != null) {
                    javaCameraView.setCvCameraViewListener(MainActivity.this);
                    javaCameraView.enableView();
                }
            } else {
                super.onManagerConnected(status);
            }
        }
    };

    @Override
    protected List<? extends CameraBridgeViewBase> getCameraViewList() {
        List<CameraBridgeViewBase> list = new ArrayList<>();
        list.add(javaCameraView);
        return list;
    }

    @Override
    public void onCameraViewStarted(int width, int height) {
        System.out.println("openCV -- start ");
        inputImage = new Mat(height, width, CvType.CV_8UC4);
    }

    @Override
    public void onCameraViewStopped() {
        System.out.println("openCV -- stop ");
        inputImage.release();
    }

    @Override
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {
        System.out.println("openCV -- frame ");
        inputImage = inputFrame.rgba();
        processCameraFrame(inputImage.getNativeObjAddr());
        return inputImage;
    }

    public native void processCameraFrame(long inputImage);

    public void detectTextWithOCR(boolean isDetect) {
        System.out.println("detectTextWithOCR -> " + isDetect);
    }
}