//
// Created by sibirsky on 30.05.18.
//

#ifndef FIRSTPROJECT_SIGNDETECTOR_H
#define FIRSTPROJECT_SIGNDETECTOR_H


#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>

class SignDetector {

public:
    SignDetector(const std::string& trainedSVMPath);
    void detect(const cv::Mat& image, std::vector<cv::Mat> detectedSigns);

private:
    cv::SVM svm;

    void findRectangles(const cv::Mat& image, std::vector<cv::Rect>& rectangles);
    cv::Point2f lineIntersection(const cv::Vec4i& a, const cv::Vec4i& b);
    void sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center);
    static bool comparator(cv::Point2f a, cv::Point2f b);

};


#endif //FIRSTPROJECT_SIGNDETECTOR_H
