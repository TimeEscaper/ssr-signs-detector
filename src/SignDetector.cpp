//
// Created by sibirsky on 30.05.18.
//

#include "../include/SignDetector.h"
#include "../include/Util.h"

SignDetector::SignDetector(const std::string &trainedSVMPath) {
    svm.load(trainedSVMPath.c_str());
}

void SignDetector::detect(const cv::Mat &image, std::vector<cv::Mat> detectedSigns) {
    cv::Mat workingImage;
    cv::GaussianBlur(image, workingImage, cv::Size(3,3), 0);
    cv::Canny(workingImage, workingImage, 50, 200);

    std::vector<cv::Mat> rects;
    findRectangles(workingImage, rects);
}

void SignDetector::findRectangles(const cv::Mat& image, std::vector<cv::Mat> rectangles) {
    std::vector<cv::Vec4i> lines;
    cv::Mat canvas;
    cv::cvtColor(image, canvas, CV_GRAY2BGR);
    cv::HoughLinesP(image, lines, 1, CV_PI/180, 15);
    /**
    for (int i = 0; i < lines.size(); i++) {
        cv::line(canvas, cv::Point2f(lines[i][0], lines[i][1]), cv::Point2f(lines[i][2], lines[i][3]), cv::Scalar(255,0,0), 5);
    }
    Util::showImage(canvas);
    */

}

cv::Point2f SignDetector::lineIntersection(const cv::Vec4i &a, const cv::Vec4i &b) {

}