#include <jni.h>
#include <string>
#include "opencv2/core.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <vector>
#include <android/log.h>

using namespace cv;
using namespace std;

void
extractSubMat(Mat &mat, Rect rect, int startY, int endY, Scalar scalarColor, jboolean isRotate) {
    Point pt1 = Point(rect.x, startY);
    Point pt2 = Point(rect.x + rect.width, rect.y + endY);
    Rect rect1 = Rect(pt1, pt2);
    cv::rectangle(mat, rect1, scalarColor, 5);
//    rectangle(mat, rect1.tl(), rect1.br(), scalarColor, 5);
    if (rect1.height > 100 && rect1.width > 150) {
        Mat subMat = mat(rect1);
        if (isRotate) {
            flip(subMat, subMat, -1);
        }
        __android_log_print(ANDROID_LOG_ERROR, "Callback JNI", "end");
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_lienserecognition_1opencv_1demo_MainActivity_processCameraFrame(JNIEnv *env,
                                                                                 jobject obj,
                                                                                 jlong srcImage) {
    __android_log_print(ANDROID_LOG_ERROR, "Callback JNI", "begin");
    Mat &srcMat = *(Mat *) srcImage;
    Mat inputSrc = srcMat.clone();
    Scalar lowerColor = Scalar(22, 93, 0);
    Scalar upperColor = Scalar(45, 255, 255);
    vector<vector<Point>> largest_contours;
    vector<vector<Point>> contours;
    cvtColor(inputSrc, inputSrc, COLOR_RGB2HSV);
    GaussianBlur(inputSrc, inputSrc, Size(9, 11), 0, 0);
    inRange(inputSrc, lowerColor, upperColor, inputSrc);
    findContours(inputSrc, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    if (contours.empty()) {
        return;
    }
    double largest_area, area;
    for (auto &contour : contours) {
        area = fabs(contourArea(contour));
        if (area >= largest_area) {
            largest_area = area;
            largest_contours.clear();
            largest_contours.push_back(contour);
        }
    }
    Scalar redColor = Scalar(255, 0, 0);
    Scalar greenColor = Scalar(104, 191, 50);
    if (largest_area >= 3000) {
//        drawContours(srcMat, largest_contours, -1, redColor, 5);
        Rect rect = boundingRect((const _InputArray &) largest_contours.back());
        extractSubMat(srcMat, rect, rect.y,
                      rect.height / 2, redColor, false);
        extractSubMat(srcMat, rect, rect.y + rect.height / 2,
                      rect.height, greenColor, true);
    }
}