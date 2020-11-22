#include <jni.h>
#include <string>
#include "opencv2/core.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <vector>

void extractSubMat(cv::Mat &mat, cv::Rect rect, int startY, int endY, cv::Scalar scalarColor) {
    cv::Point pt1 = cv::Point(rect.x, startY);
    cv::Point pt2 = cv::Point(rect.x + rect.width, rect.y + endY);
    cv::Rect rect1 = cv::Rect(pt1, pt2);
    cv::rectangle(mat, rect1, scalarColor, 5);
}

using namespace cv;
using namespace std;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_lienserecognition_1opencv_1demo_MainActivity_processCameraFrame(JNIEnv *env,
                                                                                 jobject thiz,
                                                                                 jlong srcImage) {
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
        extractSubMat(srcMat, rect, rect.y, rect.height / 2, redColor);
        extractSubMat(srcMat, rect, rect.y + rect.height / 2, rect.height, greenColor);
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_lienserecognition_1opencv_1demo_MainActivity_detectTextWithOCR(JNIEnv *env,
                                                                                jobject thiz,
                                                                                jobject rect) {

}